#include "CPG_Input_BasicCollector.h"

#include "CPG_InputKeyMap.h"
#include "step_rain_of_chaos_input_KeyCodeCollector.h"

USING_NS_CC;

namespace cpg_input
{
	BasicCollector::BasicCollector( const KeyMapSp& key_map_container ) : iKeyCollector( key_map_container )
		, mKeyHistory()
		, mCurrent_KeyStatus_Container()
		, mLast_KeyStatus_Container()
	{
		mLast_KeyStatus_Container = mKeyHistory.begin();
		mCurrent_KeyStatus_Container = mLast_KeyStatus_Container + 1;
	}

	KeyCollectorSp BasicCollector::create( const KeyMapSp& key_map_container )
	{
		KeyCollectorSp ret( new ( std::nothrow ) BasicCollector( key_map_container ) );
		return ret;
	}

	void BasicCollector::collect( const step_rain_of_chaos::input::KeyCodeCollector& key_code_collector )
	{
		for( const auto k : mKeyMapContainer->mContainer )
			( *mCurrent_KeyStatus_Container )[k.idx] = key_code_collector.isActiveKey( k.keycode );
	}
	void BasicCollector::update_forHistory()
	{
		if( mLast_KeyStatus_Container->to_ulong() != mCurrent_KeyStatus_Container->to_ulong() )
		{
			mLast_KeyStatus_Container = mCurrent_KeyStatus_Container;
			++mCurrent_KeyStatus_Container;
			if( mKeyHistory.end() == mCurrent_KeyStatus_Container )
				mCurrent_KeyStatus_Container = mKeyHistory.begin();
			*mCurrent_KeyStatus_Container = *mLast_KeyStatus_Container;
		}
	}

	const bool BasicCollector::getKeyStatus( const cocos2d::EventKeyboard::KeyCode keycode ) const
	{
		for( auto& k : mKeyMapContainer->mContainer )
			if( keycode == k.keycode )
				return ( *mCurrent_KeyStatus_Container )[k.idx];

		return false;
	}
	const bool BasicCollector::getKeyStatus( const int target_key_index ) const
	{
		if( 0 > target_key_index || static_cast<std::size_t>( target_key_index ) >= ( *mCurrent_KeyStatus_Container ).size() )
			return false;

		return ( *mCurrent_KeyStatus_Container )[target_key_index];
	}
	const bool BasicCollector::hasChanged() const
	{
		return mLast_KeyStatus_Container->to_ulong() != mCurrent_KeyStatus_Container->to_ulong();
	}
}