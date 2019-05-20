/*
  VitaTweaks: Custom Warning
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
#include <psp2/kernel/clib.h>
#include <psp2/kernel/rng.h>
#include <psp2/io/dirent.h>
#include <psp2/io/fcntl.h>
#include <psp2/sysmodule.h>
#include <psp2/paf.h>

#include <stdio.h>
#include <string.h>

#include <taihen.h>

static tai_hook_ref_t sceSysmoduleLoadModuleInternalWithArgRef;
static tai_hook_ref_t scePafToplevelGetTextRef;

static SceUID hooks[2];

static wchar_t *custom_warning = NULL;

static wchar_t *scePafToplevelGetTextPatched(void *a0, void *a1) {
  if (a1) {
    uint32_t id = *(uint32_t *)(a1 + 0xC);
    if (id == 0x76A1B071 || id == 0x72411882)
      return custom_warning;
  }

  return TAI_CONTINUE(wchar_t *, scePafToplevelGetTextRef, a0, a1);
}

static SceUID get_warn_file(const char * mount) {
  // search mount m for dir of files, then fallback to just file
  SceSize len = sceClibStrnlen(mount, 10);
  char buf [300];
  const char* loc = buf;
  SceUID dfd, fd;

  sceClibSnprintf(buf, len + 20, "%s:tai/custom_warning", mount);
  dfd = sceIoDopen(loc);
  if (dfd >= 0) {
    // look for files in folder
    SceIoDirent entry;
    uint max = 0;
    while (sceIoDread(dfd, &entry) > 0) {
      max++;
    }
    uint rando;
    if (sceKernelGetRandomNumber(&rando, sizeof(rando)) == 0) {
      uint choice = rando % max;

      // reopen to reset cursor
      sceIoDclose(dfd);
      dfd = sceIoDopen(loc);
      uint i = 0;
      while (i <= choice && sceIoDread(dfd, &entry) > 0) {
        i++;
      }
      if (i == choice) {
        SceSize str_len = sceClibStrnlen(entry.d_name, 255);
        sceClibSnprintf(buf, len + 21 + str_len, "%s:tai/custom_warning/%s", mount, entry.d_name);
        fd = sceIoOpen(loc, SCE_O_RDONLY, 0);

        sceIoDclose(dfd);
        return fd;
      }
    }
    sceIoDclose(dfd);
  }

  sceClibSnprintf(buf, len + 24, "%s:tai/custom_warning.txt", mount);
  fd = sceIoOpen(loc, SCE_O_RDONLY, 0);
  return fd;
}

static int sceSysmoduleLoadModuleInternalWithArgPatched(SceUInt32 id, SceSize args, void *argp, void *unk) {
  int res = TAI_CONTINUE(int, sceSysmoduleLoadModuleInternalWithArgRef, id, args, argp, unk);

  if (res >= 0 && id == SCE_SYSMODULE_INTERNAL_PAF) {

    // search for right file
    SceUID fd = get_warn_file("ux0");
    if (fd < 0)
      fd = get_warn_file("ur0");
    if (fd < 0)
      return res;

    int size = sceIoLseek32(fd, 0, SCE_SEEK_END);
    sceIoLseek32(fd, 0, SCE_SEEK_SET);

    custom_warning = (wchar_t *)sce_paf_private_malloc(size + 2);
    if (!custom_warning) {
      sceIoClose(fd);
      return res;
    }

    sceIoRead(fd, custom_warning, size);
    sceIoClose(fd);

    custom_warning[size / 2] = 0;

    if (custom_warning[0] != 0xFEFF) {
      sce_paf_private_free(custom_warning);
      return res;
    }

    custom_warning++;

    hooks[1] = taiHookFunctionImport(&scePafToplevelGetTextRef, "SceShell", 0x4D9A9DD0, 0x19CEFDA7,
                                     scePafToplevelGetTextPatched);
  }

  return res;
}

void _start() __attribute__ ((weak, alias("module_start")));
int module_start(SceSize args, void *argp) {
  hooks[0] = taiHookFunctionImport(&sceSysmoduleLoadModuleInternalWithArgRef, "SceShell", 0x03FCF19D, 0xC3C26339,
                                    sceSysmoduleLoadModuleInternalWithArgPatched);

  return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize args, void *argp) {
  if (hooks[1] >= 0)
    taiHookRelease(hooks[1], scePafToplevelGetTextRef);
  if (hooks[0] >= 0)
    taiHookRelease(hooks[0], sceSysmoduleLoadModuleInternalWithArgRef);

  return SCE_KERNEL_STOP_SUCCESS;
}
