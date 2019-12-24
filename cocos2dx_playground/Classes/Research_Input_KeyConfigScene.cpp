#include "Research_Input_KeyConfigScene.h"

#include <sstream>

#include "RootScene.h"
#include "CPG_InputDelegator.h"
#include "CPG_Input_BasicCollector.h"
#include "CPG_InputKeyMap.h"
#include "CPG_Input_KeyCodeNames.h"

#include "ui/UIButton.h"
#include "ui/UIScale9Sprite.h"
#include "ui/UIScrollView.h"

USING_NS_CC;

namespace Research
{
	namespace Input
	{
		namespace
		{
			const int TAG_KeyConfigControl_BG = 20140416;

			const Size calculateSizeOfKeyConfigControl( CPG::Input::KeyMapConfigHelper& _helper )
			{
				const Size control_side_margin( 8.f, 4.f );
				const float inner_horizontal_margin = 10.f;

				auto label = Label::createWithTTF( "", "fonts/arial.ttf", 10 );
				Size result_size;
				for( const auto& h : _helper.getContainer() )
				{
					label->setString( h.name );

					if( result_size.width < label->getContentSize().width )
						result_size.width = label->getContentSize().width;

					if( result_size.height < label->getContentSize().height )
						result_size.height = label->getContentSize().height;
				}

				label->setString( CPG::Input::KeyCodeNames::get_longest() );
				if( result_size.width < label->getContentSize().width )
					result_size.width = label->getContentSize().width;
				if( result_size.height < label->getContentSize().height )
					result_size.height = label->getContentSize().height;

				return Size(
					std::ceilf( ( result_size.width * 2 ) + ( control_side_margin.width * 2 ) + inner_horizontal_margin )
					, std::ceilf( result_size.height + ( control_side_margin.height * 2 ) )
				);
			}

			Node* createKeyConfigControl( const Size _control_size, const std::string& _key_name, const int _key_idx, const EventKeyboard::KeyCode _key_code, const ui::Widget::ccWidgetTouchCallback& _callback )
			{
				auto root = Node::create();
				root->setContentSize( _control_size );
				{
					auto key_name_label = Label::createWithTTF( _key_name, "fonts/arial.ttf", 10, Size::ZERO, TextHAlignment::CENTER );
					key_name_label->setPositionX( -_control_size.width * 0.25f );
					root->addChild( key_name_label, 2 );

					auto key_code_label = Label::createWithTTF( CPG::Input::KeyCodeNames::get( _key_code ), "fonts/arial.ttf", 10, Size::ZERO, TextHAlignment::CENTER );
					key_code_label->setPositionX( _control_size.width * 0.25f );
					root->addChild( key_code_label, 2 );

					auto button = ui::Button::create( "textures/ui/guide_01_1.png", "textures/ui/guide_01_2.png", "textures/ui/guide_01_1.png", ui::Widget::TextureResType::LOCAL );
					button->setTag( static_cast<int>( _key_idx ) );
					button->getRendererNormal()->getTexture()->setAliasTexParameters();
					button->getRendererClicked()->getTexture()->setAliasTexParameters();
					button->getRendererDisabled()->getTexture()->setAliasTexParameters();
					button->setScale9Enabled( true );
					button->setContentSize( _control_size );
					button->addTouchEventListener( _callback );
					root->addChild( button, 1 );

					auto indicator = ui::Scale9Sprite::create( "textures/ui/guide_01_3.png" );
					indicator->setTag( TAG_KeyConfigControl_BG );
					indicator->setVisible( false );
					indicator->setContentSize( _control_size );
					root->addChild( indicator, 0 );
				}

				return root;
			}
		}

		KeyConfigScene::KeyConfigScene() :
			input_collector()
			, keymap_config_helper()
			, current_button_node( nullptr )
		{}

		Scene* KeyConfigScene::create()
		{
			auto ret = new ( std::nothrow ) KeyConfigScene();
			if( !ret || !ret->init() )
			{
				delete ret;
				ret = nullptr;
				return nullptr;
			}

			const auto visibleSize = Director::getInstance()->getVisibleSize();
			const Vec2 origin = Director::getInstance()->getVisibleOrigin();

			//
			// summury
			//
			{
				std::stringstream ss;
				ss << "+ Input : Key Config Scene";
				ss << "\n";
				ss << "\n";
				ss << "[ESC] : Exit";
				ss << "\n";
				ss << "\n";
				ss << "<Config File Path> : " << cocos2d::FileUtils::getInstance()->getWritablePath();

				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
				label->setColor( Color3B::GREEN );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition( Vec2(
					origin.x
					, origin.y + visibleSize.height
				) );
				ret->addChild( label, 9999 );
			}

			//
			// input
			//
			{
				auto input_delegator = CPG::Input::Delegator::create( "research_input_allowedKeysTest_allowed_keys.json" );
				ret->addChild( input_delegator, 0 );

				auto key_map = CPG::Input::KeyMap::create( "research_input_keyconfigscene_keymap.json" );

				ret->input_collector = CPG::Input::BasicCollector::create( key_map );
				input_delegator->addInputCollector( ret->input_collector );
			}

			//
			// Setup Key Config Controls
			//
			{
				ret->keymap_config_helper.load( "research_input_keyconfigscene_keymap.json" );

				static const Size size_of_key_config_control = calculateSizeOfKeyConfigControl( ret->keymap_config_helper );
				const Size side_margin( 8.f, 8.f );
				const float inner_margin = 4.f;
				const float total_height = (
					( size_of_key_config_control.height * ret->keymap_config_helper.getContainer().size() )
					+ ( inner_margin * std::max( 0, static_cast<int>( ret->keymap_config_helper.getContainer().size() ) - 1 )  )
					+ ( side_margin.height * 2 )
				);
				const float start_x = origin.x + ( visibleSize.width * 0.5f );
				const float start_y = origin.y + side_margin.height + ( size_of_key_config_control.height * 0.5f );

				auto scroll_view = ui::ScrollView::create();
				scroll_view->setDirection( ui::ScrollView::Direction::VERTICAL );
				scroll_view->setContentSize( visibleSize );
				ret->addChild( scroll_view );
				{
					Node* root_node = Node::create();
					root_node->setContentSize( Size(
						visibleSize.width
						, total_height
					) );
					root_node->setPositionY(
						total_height < scroll_view->getContentSize().height
						? ( scroll_view->getContentSize().height - total_height ) * 0.5f
						: 0.f
					);
					scroll_view->addChild( root_node );
					scroll_view->setInnerContainerSize( root_node->getContentSize() );
					{
						int count = 0;
						for( const auto& h : ret->keymap_config_helper.getContainer() )
						{
							auto control = createKeyConfigControl( size_of_key_config_control, h.name, h.idx, h.keycode, CC_CALLBACK_2( KeyConfigScene::onKeyConfigControl, ret ) );
							control->setPosition( Vec2(
								start_x
								, start_y + ( ( control->getContentSize().height + inner_margin ) * count )
							) );
							root_node->addChild( control );

							++count;
						}
					}
				}
			}


			ret->autorelease();
			ret->scheduleUpdate();

			return ret;
		}

		void KeyConfigScene::update( float dt )
		{
			if( input_collector->getKeyStatus( cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE ) )
			{
				Director::getInstance()->replaceScene( RootScene::create() );
			}
		}

		void KeyConfigScene::onKeyConfigControl( Ref* _sender, ui::Widget::TouchEventType _touch_event_type )
		{
			if( ui::Widget::TouchEventType::ENDED != _touch_event_type )
				return;

			auto button_node = static_cast<Node*>( _sender );
			
			if( current_button_node )
			{
				if( current_button_node != button_node )
				{
					auto bg = current_button_node->getParent()->getChildByTag( TAG_KeyConfigControl_BG );
					bg->setVisible( false );
				}
			}

			auto button_bg = button_node->getParent()->getChildByTag( TAG_KeyConfigControl_BG );
			button_bg->setVisible( !button_bg->isVisible() );

			if( button_bg->isVisible() )
				current_button_node = button_node;
			else
				current_button_node = nullptr;
		}
	}
}
