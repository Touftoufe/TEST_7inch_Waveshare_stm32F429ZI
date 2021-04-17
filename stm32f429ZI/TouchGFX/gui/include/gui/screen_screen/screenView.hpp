#ifndef SCREENVIEW_HPP
#define SCREENVIEW_HPP

#include <gui_generated/screen_screen/screenViewBase.hpp>
#include <gui/screen_screen/screenPresenter.hpp>

class screenView : public screenViewBase
{
public:
    screenView();
    virtual ~screenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void changeButtonColor();
    virtual void circleUpdateValue(int newCircleValue);
    virtual void circleUpdateColor(int r, int g, int b);
protected:
};

#endif // SCREENVIEW_HPP
