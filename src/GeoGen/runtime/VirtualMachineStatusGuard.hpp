/* GeoGen - Programmable height map generator
Copyright (C) 2015  Matej Zabsky

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#pragma once

#include "VirtualMachine.hpp"

namespace geogen
{
	namespace runtime
	{
		/// Guard that sets a status of a VirtualMachine to a specified value when it is destroyed. Used mark VM as failed when native execution leaves a block prematurely, for example because an exception was thrown.
		class VirtualMachineStatusGuard
		{
		private:
			VirtualMachineStatus guardStatus;
			VirtualMachineStatus& vmStatus;
		public:

			/// Constructor.
			/// @param vmStatus Reference to a field with VM status to be set when this object is destroyed.
			VirtualMachineStatusGuard(VirtualMachineStatus& vmStatus) : guardStatus(VIRTUAL_MACHINE_STATUS_FAULTED), vmStatus(vmStatus) {}
			~VirtualMachineStatusGuard()
			{
				this->vmStatus = this->guardStatus;
			}

			/// Sets status to which the VM is set when this object is destroyed. Unless this is called, VIRTUAL_MACHINE_STATUS_FAULTED will be used.
			/// @param status The status.
			void SetGuardStatus(VirtualMachineStatus status)
			{
				this->guardStatus = status;
			}
		};
	}
}