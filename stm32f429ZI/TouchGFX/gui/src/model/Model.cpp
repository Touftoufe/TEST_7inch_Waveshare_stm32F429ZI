#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

volatile int circleValueUpdate = 0;
volatile int circleValueReset  = 0;

Model::Model() : modelListener(0)
{

}

void Model::tick()
{
	if(circleValueReset) {
		circleValueReset = 0;
		currentCircleValue = 0;
		if (modelListener != 0)
		{
			modelListener->notifyCircleValueChanged(currentCircleValue);
		}
	}
	else if(circleValueUpdate && currentCircleValue < 100) {
		++currentCircleValue, circleValueUpdate = 0;

		if (modelListener != 0)
		{
			modelListener->notifyCircleValueChanged(currentCircleValue);
		}
	}

}
