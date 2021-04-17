#include <gui/screen_screen/screenView.hpp>
#include <touchgfx/Color.hpp>

screenView::screenView()
{

}

void screenView::setupScreen()
{
    screenViewBase::setupScreen();
}

void screenView::tearDownScreen()
{
    screenViewBase::tearDownScreen();
}

void screenView::changeButtonColor(){
	static int x = 0;
	x = ~x;
	__background.setColor(x);
	__background.invalidate();
	textArea1.setColor(~x);
	textArea1.invalidate();
}


void screenView::circleUpdateValue(int newCircleValue){
	circleProgress1.setValue(newCircleValue);
	circleProgress1.invalidate();
}

void screenView::circleUpdateColor(int r, int g, int b){
	circleProgress1Painter.setColor(touchgfx::Color::getColorFrom24BitRGB(r, g, b));
	circleProgress1.invalidate();

}
