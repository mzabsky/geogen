#pragma once

#include "VirtualMachine.hpp"

namespace geogen
{
	namespace runtime
	{
		class VirtualMachineStatusGuard
		{
		private:
			VirtualMachineStatus guardStatus;
			VirtualMachineStatus& vmStatus;
		public:
			VirtualMachineStatusGuard(VirtualMachineStatus& vmStatus) : guardStatus(VIRTUAL_MACHINE_STATUS_FAULTED), vmStatus(vmStatus) {}
			~VirtualMachineStatusGuard()
			{
				this->vmStatus = this->guardStatus;
			}

			void SetGuardStatus(VirtualMachineStatus status)
			{
				this->guardStatus = status;
			}
		};
	}
}