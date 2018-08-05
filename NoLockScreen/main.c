/*
  VitaTweaks: NoLockScreen
  Copyright (C) 2018, TheFloW

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <psp2/kernel/modulemgr.h>
#include <taihen.h>

static SceUID hooks[2];

void _start() __attribute__ ((weak, alias("module_start")));
int module_start(SceSize args, void *argp) {
  tai_module_info_t info;
  info.size = sizeof(info);
  if (taiGetModuleInfo("SceShell", &info) >= 0) {
    uint32_t movs_a1_1_nop_opcode = 0xBF002001;

    switch (info.module_nid) {
      case 0x0552F692: // 3.60 retail
        hooks[0] = taiInjectData(info.modid, 0, 0x2354D2, &movs_a1_1_nop_opcode, sizeof(movs_a1_1_nop_opcode));
        hooks[1] = taiInjectData(info.modid, 0, 0x2361D2, &movs_a1_1_nop_opcode, sizeof(movs_a1_1_nop_opcode));
        break;

      case 0x6CB01295: // 3.60 PDEL
        hooks[0] = taiInjectData(info.modid, 0, 0x228EF6, &movs_a1_1_nop_opcode, sizeof(movs_a1_1_nop_opcode));
        hooks[1] = taiInjectData(info.modid, 0, 0x229BF0, &movs_a1_1_nop_opcode, sizeof(movs_a1_1_nop_opcode));
        break;
      
      case 0xEAB89D5C: // 3.60 PTEL
        hooks[0] = taiInjectData(info.modid, 0, 0x22D906, &movs_a1_1_nop_opcode, sizeof(movs_a1_1_nop_opcode));
        hooks[1] = taiInjectData(info.modid, 0, 0x22E606, &movs_a1_1_nop_opcode, sizeof(movs_a1_1_nop_opcode));
        break;

      case 0x5549BF1F: // 3.65 retail
      case 0x34B4D82E: // 3.67 retail
      case 0x12DAC0F3: // 3.68 retail
        hooks[0] = taiInjectData(info.modid, 0, 0x23556E, &movs_a1_1_nop_opcode, sizeof(movs_a1_1_nop_opcode));
        hooks[1] = taiInjectData(info.modid, 0, 0x23626E, &movs_a1_1_nop_opcode, sizeof(movs_a1_1_nop_opcode));
        break;

      case 0xE6A02F2B: // 3.65 PDEL
        hooks[0] = taiInjectData(info.modid, 0, 0x228F92, &movs_a1_1_nop_opcode, sizeof(movs_a1_1_nop_opcode));
        hooks[1] = taiInjectData(info.modid, 0, 0x229C8C, &movs_a1_1_nop_opcode, sizeof(movs_a1_1_nop_opcode));
        break;

      case 0x587F9CED: // 3.65 PTEL
        hooks[0] = taiInjectData(info.modid, 0, 0x22D9A2, &movs_a1_1_nop_opcode, sizeof(movs_a1_1_nop_opcode));
        hooks[1] = taiInjectData(info.modid, 0, 0x22E6A2, &movs_a1_1_nop_opcode, sizeof(movs_a1_1_nop_opcode));
        break;
    }
  }

  return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize args, void *argp) {
  if (hooks[1] >= 0)
    taiInjectRelease(hooks[1]);
  if (hooks[0] >= 0)
    taiInjectRelease(hooks[0]);

  return SCE_KERNEL_STOP_SUCCESS;
}
