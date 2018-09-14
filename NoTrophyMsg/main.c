/*
  VitaTweaks: NoTrophyMsg
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

static tai_hook_ref_t savedata_aid_check_ref;

static SceUID hookid = -1;

static int savedata_aid_check_patched(int a1, int a2) {
  int res = TAI_CONTINUE(int, savedata_aid_check_ref, a1, a2);
  if (res == 0x80103933 || res == 0x80103935)
    res = 0;
  return res;
}

void _start() __attribute__ ((weak, alias("module_start")));
int module_start(SceSize args, void *argp) {
  tai_module_info_t info;
  info.size = sizeof(info);
  if (taiGetModuleInfo("SceShell", &info) >= 0) {
    switch (info.module_nid) {
      case 0x0552F692: // 3.60 retail
        hookid = taiHookFunctionOffset(&savedata_aid_check_ref, info.modid, 0, 0x28B854, 1, savedata_aid_check_patched);
        break;

      case 0x6CB01295: // 3.60 PDEL
        hookid = taiHookFunctionOffset(&savedata_aid_check_ref, info.modid, 0, 0x27F1F8, 1, savedata_aid_check_patched);
        break;

      case 0xEAB89D5C: // 3.60 PTEL
        hookid = taiHookFunctionOffset(&savedata_aid_check_ref, info.modid, 0, 0x283C00, 1, savedata_aid_check_patched);
        break;

      case 0x5549BF1F: // 3.65 retail
      case 0x34B4D82E: // 3.67 retail
      case 0x12DAC0F3: // 3.68 retail
      case 0x0703C828: // 3.69 retail
        hookid = taiHookFunctionOffset(&savedata_aid_check_ref, info.modid, 0, 0x28BC80, 1, savedata_aid_check_patched);
        break;

      case 0xE6A02F2B: // 3.65 PDEL
        hookid = taiHookFunctionOffset(&savedata_aid_check_ref, info.modid, 0, 0x27F6C4, 1, savedata_aid_check_patched);
        break;

      case 0x587F9CED: // 3.65 PTEL
        hookid = taiHookFunctionOffset(&savedata_aid_check_ref, info.modid, 0, 0x28402C, 1, savedata_aid_check_patched);
        break;
    }
  }

  return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize args, void *argp) {
  if (hookid >= 0)
    taiHookRelease(hookid, savedata_aid_check_ref);

  return SCE_KERNEL_STOP_SUCCESS;
}
