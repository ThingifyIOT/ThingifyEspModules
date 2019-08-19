#ifndef  _IR_MODULE_H
#define _IR_MODULE_H

#include <Conti.h>
#include "IModule.h"
#include "Libraries/IrRemote/IrRemote.h"

class IrModule: public IModule
{
	Conti& _thing;
	int _irTxPin;
	IRsend _irSender;
	Node* fanOnNode;
	Node* fanOffNode;
public:
	const char* GetName() override;
	bool Tick() override;
	bool Init() override;
	NodeValue OnFunctionExecuted(void* context, FunctionArguments& args) override;
	bool OnValueChanged(Node *node) override;
	IrModule(Conti &thing, int irTxPin);
};

#endif // ! _IR_MODULE_H

