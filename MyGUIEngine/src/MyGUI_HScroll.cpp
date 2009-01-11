/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_HScroll.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_Button.h"
#include "MyGUI_WidgetSkinInfo.h"

namespace MyGUI
{

	HScroll::HScroll(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name) :
		VScroll(_style, _coord, _align, _info, _parent, _croppedParent, _creator, _name)
	{
		initialiseWidgetSkin(_info);
	}

	HScroll::~HScroll()
	{
		shutdownWidgetSkin();
	}

	void HScroll::baseChangeWidgetSkin(WidgetSkinInfoPtr _info)
	{
		shutdownWidgetSkin();
		VScroll::baseChangeWidgetSkin(_info);
		initialiseWidgetSkin(_info);
	}

	void HScroll::initialiseWidgetSkin(WidgetSkinInfoPtr _info)
	{
	}

	void HScroll::shutdownWidgetSkin()
	{
	}

	void HScroll::updateTrack()
	{
		_forcePeek(mWidgetTrack);
		// размер диапазана в пикселях
		int pos = getLineSize();

		// скрываем если диапазан маленький или места мало
		if ((mScrollRange < 2) || (pos <= mWidgetTrack->getWidth())) {
			mWidgetTrack->hide();
			if ( nullptr != mWidgetFirstPart ) mWidgetFirstPart->setSize(pos/2, mWidgetFirstPart->getHeight());
			if ( nullptr != mWidgetSecondPart ) mWidgetSecondPart->setCoord(pos/2 + mSkinRangeStart, mWidgetSecondPart->getTop(), pos - pos/2, mWidgetSecondPart->getHeight());
			if ( pos < 0 )
			{
				//if ( nullptr != mWidgetStart ) mWidgetStart->setSize(mSkinRangeStart + pos/2, mWidgetStart->getHeight());
				//if ( nullptr != mWidgetEnd ) mWidgetEnd->setPosition(pos/2 + mSkinRangeStart, mWidgetEnd->getTop(), mCoord.height - (pos/2 + mSkinRangeStart), mWidgetEnd->getHeight());
			}
			else
			{
				//if ( nullptr != mWidgetStart ) mWidgetStart->setSize(mSkinRangeStart, mWidgetStart->getHeight());
				//if ( nullptr != mWidgetEnd ) mWidgetEnd->setPosition(mCoord.width - mSkinRangeEnd, mWidgetEnd->getTop(), mSkinRangeEnd, mWidgetEnd->getHeight());
			}
			return;
		}
		// если скрыт то покажем
		if (false == mWidgetTrack->isShow())
		{
			mWidgetTrack->show();
			//if ( nullptr != mWidgetStart ) mWidgetStart->setSize(mSkinRangeStart, mWidgetStart->getHeight());
			//if ( nullptr != mWidgetEnd ) mWidgetEnd->setPosition(mCoord.width - mSkinRangeEnd, mWidgetEnd->getTop(), mSkinRangeEnd, mWidgetEnd->getHeight());
		}

		// и обновляем позицию
		pos = (int)(((size_t)(pos-getTrackSize()) * mScrollPosition) / (mScrollRange-1) + mSkinRangeStart);

		mWidgetTrack->setPosition(pos, mWidgetTrack->getTop());
		if ( nullptr != mWidgetFirstPart )
		{
			int height = pos + mWidgetTrack->getWidth()/2 - mWidgetFirstPart->getLeft();
			mWidgetFirstPart->setSize(height, mWidgetFirstPart->getHeight());
		}
		if ( nullptr != mWidgetSecondPart )
		{
			int top = pos + mWidgetTrack->getWidth()/2;
			int height = mWidgetSecondPart->getWidth() + mWidgetSecondPart->getLeft() - top;
			mWidgetSecondPart->setCoord(top, mWidgetSecondPart->getTop(), height, mWidgetSecondPart->getHeight());
		}
	}

	void HScroll::TrackMove(int _left, int _top)
	{
		const IntPoint & point = InputManager::getInstance().getLastLeftPressed();

		// расчитываем позицию виджета
		int start = mPreActionRect.left + (_left - point.left);
		if (start < (int)mSkinRangeStart) start = (int)mSkinRangeStart;
		else if (start > (mCoord.width - (int)mSkinRangeEnd - mWidgetTrack->getWidth())) start = (mCoord.width - (int)mSkinRangeEnd - mWidgetTrack->getWidth());
		if (mWidgetTrack->getLeft() != start) mWidgetTrack->setPosition(IntPoint(start, mWidgetTrack->getTop()));

		// расчитываем положение соответствующее позиции
		// плюс пол позиции
		int pos = start - (int)mSkinRangeStart + (getLineSize() - getTrackSize()) / (((int)mScrollRange-1) * 2);
		// высчитываем ближайшее значение и обновляем
		pos = pos * (int)(mScrollRange-1) / (getLineSize() - getTrackSize());

		// проверяем на выходы и изменения
		if (pos < 0) pos = 0;
		else if (pos >= (int)mScrollRange) pos = (int)mScrollRange - 1;
		if (pos == (int)mScrollPosition) return;

		mScrollPosition = pos;
		// отсылаем событие
		eventScrollChangePosition(this, (int)mScrollPosition);
	}

	void HScroll::setTrackSize(size_t _size)
	{
		mWidgetTrack->setSize(((int)_size < (int)mMinTrackSize)? (int)mMinTrackSize : (int)_size, mWidgetTrack->getHeight());
		updateTrack();
	}

	int HScroll::getTrackSize()
	{
		return mWidgetTrack->getWidth();
	}

	int HScroll::getLineSize()
	{
		return mCoord.width - (int)(mSkinRangeStart + mSkinRangeEnd);
	}

} // namespace MyGUI
