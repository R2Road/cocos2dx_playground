#include "input_practice_KeyConfigScene.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "base/CCDirector.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventListenerKeyboard.h"
#include "platform/CCFileUtils.h"
#include "ui/UIButton.h"
#include "ui/UIScale9Sprite.h"
#include "ui/UIScrollView.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"
#include "cpg_input_KeyCodeNames.h"

#include "input_practice_Setting.h"

USING_NS_CC;

namespace
{
	const int TAG_KeyIndicator = 20140416;
	const int TAG_KeyCode_Label = 20160528;

	const Size calculateSizeOfKeyConfigControl( cpg_input::KeyMapConfigHelper& helper )
	{
		const Size control_side_margin( 8.f, 4.f );
		const float inner_horizontal_margin = 10.f;

		auto label = Label::createWithTTF( "", cpg::StringTable::GetFontPath(), 10 );
		Size result_size;
		for( const auto& h : helper.GetContainer() )
		{
			label->setString( h.Name );

			if( result_size.width < label->getContentSize().width )
			{
				result_size.width = label->getContentSize().width;
			}

			if( result_size.height < label->getContentSize().height )
			{
				result_size.height = label->getContentSize().height;
			}
		}

		label->setString( cpg_input::KeyCodeNames::Get_Longest() );
		if( result_size.width < label->getContentSize().width )
		{
			result_size.width = label->getContentSize().width;
		}
		if( result_size.height < label->getContentSize().height )
		{
			result_size.height = label->getContentSize().height;
		}

		return Size(
			std::ceilf( ( result_size.width * 2 ) + ( control_side_margin.width * 2 ) + inner_horizontal_margin )
			, std::ceilf( result_size.height + ( control_side_margin.height * 2 ) )
		);
	}

	Node* createKeyConfigControl( const Size control_size, const std::string& key_name, const int key_idx, const EventKeyboard::KeyCode key_code, const ui::Widget::ccWidgetTouchCallback& callback )
	{
		auto root = Node::create();
		root->setContentSize( control_size );
		{
			auto key_name_label = Label::createWithTTF( key_name, cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::CENTER );
			key_name_label->setPositionX( -control_size.width * 0.25f );
			root->addChild( key_name_label, 1 );

			auto key_code_label = Label::createWithTTF( cpg_input::KeyCodeNames::Get( key_code ), cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::CENTER );
			key_code_label->setTag( TAG_KeyCode_Label );
			key_code_label->setPositionX( control_size.width * 0.25f );
			root->addChild( key_code_label, 1 );

			auto button = ui::Button::create( "guide_01_0.png", "guide_01_1.png", "guide_01_0.png", ui::Widget::TextureResType::PLIST );
			button->setTag( static_cast<int>( key_idx ) );
			button->setScale9Enabled( true );
			button->setContentSize( control_size );
			button->addTouchEventListener( callback );
			root->addChild( button, 0 );

			auto indicator = ui::Scale9Sprite::createWithSpriteFrameName( "guide_01_4.png" );
			indicator->setTag( TAG_KeyIndicator );
			indicator->setVisible( false );
			indicator->setContentSize( control_size );
			root->addChild( indicator, std::numeric_limits<int>::max() );
		}

		return root;
	}
}

namespace input_practice
{
	KeyConfigScene::KeyConfigScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )

		, mAllowedKeys()
		, mKeymapConfigHelper()
		, mCurrentButtonNode( nullptr )
	{}

	Scene* KeyConfigScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) KeyConfigScene( back_to_the_previous_scene_callback );
		if( !ret || !ret->init() )
		{
			delete ret;
			ret = nullptr;
		}
		else
		{
			ret->autorelease();
		}

		return ret;
	}

	bool KeyConfigScene::init()
	{
		if( !Scene::init() )
		{
			return false;
		}

		const auto visibleSize = _director->getVisibleSize();
		const auto visibleOrigin = _director->getVisibleOrigin();

		//
		// summury
		//
		{
			std::stringstream ss;
			ss << "+ Input : Key Config Scene";
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "<Config File Path> : " << FileUtils::getInstance()->getWritablePath();

			auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
			label->setColor( Color3B::GREEN );
			label->setAnchorPoint( Vec2( 0.f, 1.f ) );
			label->setPosition( Vec2(
				visibleOrigin.x
				, visibleOrigin.y + visibleSize.height
			) );
			addChild( label, std::numeric_limits<int>::max() );
		}

		//
		// exit interface
		//
		{
			auto label = Label::createWithTTF( "Save & Exit", cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::CENTER );
			label->setColor( Color3B::GREEN );

			auto button = ui::Button::create( "guide_01_0.png", "guide_01_1.png", "guide_01_0.png", ui::Widget::TextureResType::PLIST );
			button->setColor( Color3B::GREEN );
			button->setScale9Enabled( true );
			button->setContentSize( label->getContentSize() + Size( 40.f, 4.f ) + Size( 40.f, 4.f ) );
			button->addTouchEventListener( CC_CALLBACK_2( KeyConfigScene::onExitButton, this ) );
			addChild( button, std::numeric_limits<int>::max() );
			button->setTitleLabel( label );

			button->setPosition( Vec2(
				visibleOrigin.x + visibleSize.width - ( button->getContentSize().width * 0.5f )
				, visibleOrigin.y + visibleSize.height - ( button->getContentSize().height * 0.5f )
			) );
		}

		//
		// key info
		//
		{
			cpg_input::AllowedKeys::Load( mAllowedKeys, input_practice::Setting::getKeyAllowFileName().c_str() );
			mKeymapConfigHelper.Load( input_practice::Setting::getKeyMapFileName().c_str() );
		}

		//
		// Setup Key Config Controls
		//
		{
			static const Size size_of_key_config_control = calculateSizeOfKeyConfigControl( mKeymapConfigHelper );
			const Size side_margin( 8.f, 8.f );
			const float inner_margin = 4.f;
			const float total_height = (
				( size_of_key_config_control.height * mKeymapConfigHelper.GetContainer().size() )
				+ ( inner_margin * std::max( 0, static_cast<int>( mKeymapConfigHelper.GetContainer().size() ) - 1 )  )
				+ ( side_margin.height * 2 )
			);
			const float start_x = visibleOrigin.x + ( visibleSize.width * 0.5f );
			const float start_y = visibleOrigin.y + side_margin.height + ( size_of_key_config_control.height * 0.5f );

			auto scroll_view = ui::ScrollView::create();
			scroll_view->setDirection( ui::ScrollView::Direction::VERTICAL );
			scroll_view->setContentSize( visibleSize );
			addChild( scroll_view );
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
					for( const auto& h : mKeymapConfigHelper.GetContainer() )
					{
						if( h.Name.empty() || h.SpriteFrameName.empty() )
							continue;

						auto control = createKeyConfigControl( size_of_key_config_control, h.Name, h.Index, h.CocosKeyCode, CC_CALLBACK_2( KeyConfigScene::onKeyConfigControl, this ) );
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

		return true;
	}


	void KeyConfigScene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyReleased = CC_CALLBACK_2( KeyConfigScene::onKeyReleased, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void KeyConfigScene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Scene::onExit();
	}


	void KeyConfigScene::onKeyConfigControl( Ref* sender, ui::Widget::TouchEventType touch_event_type )
	{
		if( ui::Widget::TouchEventType::ENDED != touch_event_type )
		{
			return;
		}

		auto button_node = static_cast<Node*>( sender );
			
		if( mCurrentButtonNode )
		{
			if( mCurrentButtonNode != button_node )
			{
				auto indicator = mCurrentButtonNode->getParent()->getChildByTag( TAG_KeyIndicator );
				indicator->setVisible( false );
			}
		}

		auto indicator = button_node->getParent()->getChildByTag( TAG_KeyIndicator );
		indicator->setVisible( !indicator->isVisible() );

		if( indicator->isVisible() )
		{
			mCurrentButtonNode = button_node;
		}
		else
		{
			mCurrentButtonNode = nullptr;
		}
	}


	void KeyConfigScene::onKeyReleased( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		if( !mCurrentButtonNode )
		{
			return;
		}

		if( !mAllowedKeys[static_cast<std::size_t>( keycode )] )
		{
			return;
		}

		mKeymapConfigHelper.Set( mCurrentButtonNode->getTag(), keycode );

		auto label = static_cast<Label*>( mCurrentButtonNode->getParent()->getChildByTag( TAG_KeyCode_Label ) );
		label->setString( cpg_input::KeyCodeNames::Get( keycode ) );
	}


	void KeyConfigScene::onExitButton( Ref* /*sender*/, ui::Widget::TouchEventType touch_event_type )
	{
		if( ui::Widget::TouchEventType::ENDED == touch_event_type )
		{
			mKeymapConfigHelper.Save( input_practice::Setting::getKeyMapFileName().c_str() );

			helper::BackToThePreviousScene::MoveBack();

			return;
		}
	}
}
