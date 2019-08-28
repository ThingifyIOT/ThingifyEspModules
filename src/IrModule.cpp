#include "IrModule.h"

IrModule::IrModule(Conti& thing, int irTxPin)
	:_thing(thing), _irTxPin(irTxPin), _irSender(irTxPin)
{
}


const char* IrModule::GetName()
{
	
	return "IR";
}
bool IrModule::Tick()
{
	if (_thing["Candle"]->Value.AsInt() == 2)
	{
		static SoftTimer st(700);
		static bool candleState = false;
		if (st.IsElapsed())
		{
			candleState = !candleState;
			if (candleState)
			{
				_irSender.sendNEC(0xFF00FF, 32);
			}
			else
			{
				_irSender.sendNEC(0xFF807F, 32);
			}
		}
	}
	return true;
}
bool IrModule::Init()
{
	_thing.AddBoolean("Candle")->OnChanged(this, ValueChangedHandler);
	//fanOnNode = _thing.AddFunction("Fan On", FunctionExecutionHandler, this);
	//fanOffNode = _thing.AddFunction("Fan Off", FunctionExecutionHandler, this);
	
	return true;
}

bool IrModule::OnValueChanged(Node *node)
{
	if (node->Value.AsBool())
	{
		_irSender.sendNEC(0xFF00FF, 32);
	}
	else
	{
		_irSender.sendNEC(0xFF807F, 32);
	}	
	return true;
}

NodeValue IrModule::OnFunctionExecuted(void* context, FunctionArguments& args)
{
	unsigned int  onSignal[23] = {450, 1250,450, 350,1300, 1250,450, 1250,450, 400,1250, 450,1250, 400,1300, 400,1250, 450,1250, 1300,400, 400 };  // UNKNOWN 143226DB
	unsigned int  offSignal[23] = { 1300,400, 1300,400, 450,1250, 1250,400, 1300,400, 450,1250, 450,1250, 400,1300, 400,1300, 400,1250, 450,1250, 1250 };  // UNKNOWN A32AB931
	Serial.println("Fan on");

	_irSender.sendRaw(onSignal, sizeof(onSignal) / sizeof(onSignal[0]), 38); //Note the approach used to automatically calculate the size of the array.


	return NodeValue::Boolean(true);
}
