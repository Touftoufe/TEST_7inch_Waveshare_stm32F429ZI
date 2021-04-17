#include <gui/screen_screen/screenView.hpp>
#include <gui/screen_screen/screenPresenter.hpp>

screenPresenter::screenPresenter(screenView& v)
    : view(v)
{

}

void screenPresenter::activate()
{

}

void screenPresenter::deactivate()
{

}

void screenPresenter::notifyCircleValueChanged(int newCircleValue) {
        	view.circleUpdateValue(newCircleValue);
        	if(newCircleValue == 100){
        		view.circleUpdateColor(0, 200, 100);
        	}
        	else if (newCircleValue == 0) view.circleUpdateColor(0, 138, 255);

      }
