#include "YieldAsNamedInstruction.hpp"
#include "..\CodeBlockStackEntry.hpp"
#include "..\VirtualMachine.hpp"
#include "..\..\InternalErrorException.hpp"
#include "..\ManagedObject.hpp"
#include "..\..\corelib\YieldRenderingStep.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::runtime;
using namespace geogen::runtime::instructions;
using namespace geogen::corelib;
using namespace geogen::renderer;

const String YieldAsNamedInstruction::MAP_NAME_MAIN = GG_STR("main");

InstructionStepResult YieldAsNamedInstruction::Step(VirtualMachine* vm) const
{
	vm->GetObjectStack().CheckSize(1);
	ManagedObject* yieldedValue = vm->GetObjectStack().Top();
	yieldedValue->AddRef();
	vm->GetObjectStack().Pop(vm);

	vector<unsigned> argumentSlots;
	unsigned returnObjectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(yieldedValue);
	RenderingStep* renderingStep = new YieldRenderingStep(this->GetLocation(), argumentSlots, returnObjectSlot, this->functionName, Rectangle(Point(vm->GetArguments().GetRenderOriginX(), vm->GetArguments().GetRenderOriginY()), Size2D(vm->GetArguments().GetRenderWidth(), vm->GetArguments().GetRenderHeight())));
	vm->GetRenderingSequence().AddStep(renderingStep);

	yieldedValue->RemoveRef(vm->GetMemoryManager());

	return INSTRUCTION_STEP_RESULT_TYPE_NORMAL;
}