// Eets decompilation part 8/8 (functions 7001-7395)

// ======== luaB_getn @ 00646690 ========

undefined8 luaB_getn(undefined8 param_1)

{
  int iVar1;
  
  luaL_checktype(param_1,1,5);
  iVar1 = luaL_getn(param_1,1);
  lua_pushnumber((double)iVar1,param_1);
  return 1;
}



// ======== luaB_setn @ 006466d0 ========

undefined8 luaB_setn(undefined8 param_1)

{
  double dVar1;
  
  luaL_checktype(param_1,1,5);
  dVar1 = (double)luaL_checknumber(param_1,2);
  luaL_setn(param_1,1,(int)dVar1);
  return 0;
}



// ======== luaB_sort @ 00646710 ========

undefined8 luaB_sort(undefined8 param_1)

{
  undefined4 uVar1;
  int iVar2;
  
  luaL_checktype(param_1,1,5);
  uVar1 = luaL_getn(param_1,1);
  luaL_checkstack(param_1,0x28,&DAT_0066a1bb);
  iVar2 = lua_type(param_1,2);
  if (0 < iVar2) {
    luaL_checktype(param_1,2,6);
  }
  lua_settop(param_1,2);
  auxsort(param_1,1,uVar1);
  return 0;
}



// ======== luaB_tinsert @ 00646790 ========

undefined8 luaB_tinsert(undefined8 param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined8 uVar4;
  double dVar5;
  
  iVar1 = lua_gettop();
  luaL_checktype(param_1,1,5);
  iVar2 = luaL_getn(param_1,1);
  iVar3 = iVar2 + 1;
  if (iVar1 == 2) {
    luaL_setn(param_1,1,iVar3);
    uVar4 = 2;
    iVar1 = iVar3;
  }
  else {
    dVar5 = (double)luaL_checknumber(param_1,2);
    iVar1 = (int)dVar5;
    if (iVar3 < iVar1) {
      iVar3 = iVar1;
    }
    luaL_setn(param_1,1,iVar3);
    uVar4 = 3;
    if (iVar1 <= iVar2) {
      do {
        iVar2 = iVar3 + -1;
        lua_rawgeti(param_1,1,iVar2);
        lua_rawseti(param_1,1,iVar3);
        iVar3 = iVar2;
      } while (iVar1 < iVar2);
    }
  }
  lua_pushvalue(param_1,uVar4);
  lua_rawseti(param_1,1,iVar1);
  return 0;
}



// ======== luaB_tremove @ 00646870 ========

undefined8 luaB_tremove(undefined8 param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined8 uVar4;
  double dVar5;
  
  luaL_checktype(param_1,1,5);
  iVar2 = luaL_getn(param_1,1);
  dVar5 = (double)luaL_optnumber((double)iVar2,param_1,2);
  if (iVar2 < 1) {
    uVar4 = 0;
  }
  else {
    iVar3 = (int)dVar5;
    uVar4 = 1;
    luaL_setn(param_1,1,iVar2 + -1);
    lua_rawgeti(param_1,1,iVar3);
    if (iVar3 < iVar2) {
      do {
        iVar1 = iVar3 + 1;
        lua_rawgeti(param_1,1,iVar1);
        lua_rawseti(param_1,1,iVar3);
        iVar3 = iVar1;
      } while (iVar2 != iVar1);
    }
    lua_pushnil(param_1);
    lua_rawseti(param_1,1,iVar2);
  }
  return uVar4;
}



// ======== auxsort @ 00646940 ========

void auxsort(undefined8 param_1,int param_2,int param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  if (param_2 < param_3) {
    do {
      lua_rawgeti(param_1,1,param_2);
      lua_rawgeti(param_1,1,param_3);
      iVar2 = sort_comp(param_1,0xffffffff,0xfffffffe);
      if (iVar2 == 0) {
        lua_settop(param_1,0xfffffffd);
      }
      else {
        lua_rawseti(param_1,1,param_2);
        lua_rawseti(param_1,1,param_3);
      }
      if (param_3 - param_2 == 1) {
        return;
      }
      iVar2 = (param_2 + param_3) / 2;
      lua_rawgeti(param_1,1,iVar2);
      lua_rawgeti(param_1,1,param_2);
      iVar3 = sort_comp(param_1,0xfffffffe,0xffffffff);
      if (iVar3 == 0) {
        lua_settop(param_1,0xfffffffe);
        lua_rawgeti(param_1,1,param_3);
        iVar3 = sort_comp(param_1,0xffffffff,0xfffffffe);
        if (iVar3 == 0) {
          lua_settop(param_1,0xfffffffd);
        }
        else {
          lua_rawseti(param_1,1,iVar2);
          lua_rawseti(param_1,1,param_3);
        }
      }
      else {
        lua_rawseti(param_1,1,iVar2);
        lua_rawseti(param_1,1,param_2);
      }
      if (param_3 - param_2 == 2) {
        return;
      }
      lua_rawgeti(param_1,1,iVar2);
      lua_pushvalue(param_1,0xffffffff);
      iVar4 = param_3 + -1;
      lua_rawgeti(param_1,1,iVar4);
      iVar3 = iVar4;
      iVar5 = param_2;
      do {
        lua_rawseti(param_1,1,iVar2);
        lua_rawseti(param_1,1,iVar3);
        iVar1 = iVar5;
        while( true ) {
          iVar5 = iVar1 + 1;
          lua_rawgeti(param_1,1,iVar5);
          iVar2 = lua_type(param_1,2);
          if (iVar2 == 0) {
            iVar2 = lua_lessthan(param_1,0xffffffff,0xfffffffe);
          }
          else {
            lua_pushvalue(param_1,2);
            lua_pushvalue(param_1,0xfffffffe);
            lua_pushvalue(param_1,0xfffffffc);
            lua_call(param_1,2,1);
            iVar2 = lua_toboolean(param_1,0xffffffff);
            lua_settop(param_1,0xfffffffe);
          }
          iVar6 = iVar3;
          if (iVar2 == 0) break;
          if (param_3 <= iVar1) {
            luaL_error(param_1,"invalid order function for sorting");
          }
          lua_settop(param_1,0xfffffffe);
          iVar1 = iVar5;
        }
        while( true ) {
          lua_rawgeti(param_1,1,iVar6 + -1);
          iVar2 = lua_type(param_1,2);
          if (iVar2 == 0) {
            iVar2 = lua_lessthan(param_1,0xfffffffd,0xffffffff);
          }
          else {
            lua_pushvalue(param_1,2);
            lua_pushvalue(param_1,0xfffffffc);
            lua_pushvalue(param_1,0xfffffffd);
            lua_call(param_1,2,1);
            iVar2 = lua_toboolean(param_1,0xffffffff);
            lua_settop(param_1,0xfffffffe);
          }
          if (iVar2 == 0) break;
          if (iVar6 <= param_2) {
            luaL_error(param_1,"invalid order function for sorting");
          }
          lua_settop(param_1,0xfffffffe);
          iVar6 = iVar6 + -1;
        }
        iVar3 = iVar6 + -1;
        iVar2 = iVar5;
      } while (iVar5 < iVar6);
      lua_settop(param_1,0xfffffffc);
      lua_rawgeti(param_1,1,iVar4);
      lua_rawgeti(param_1,1,iVar5);
      lua_rawseti(param_1,1,iVar4);
      lua_rawseti(param_1,1,iVar5);
      iVar3 = iVar1 + 2;
      iVar2 = param_3;
      iVar4 = iVar1;
      if (iVar5 - param_2 < param_3 - iVar5) {
        iVar3 = param_2;
        iVar2 = iVar1;
        iVar4 = param_3;
        param_2 = iVar1 + 2;
      }
      param_3 = iVar4;
      auxsort(param_1,iVar3,iVar2);
    } while (param_2 < param_3);
  }
  return;
}



// ======== sort_comp @ 00646dd0 ========

ulong sort_comp(undefined8 param_1,int param_2,int param_3)

{
  int iVar1;
  uint uVar2;
  ulong uVar3;
  
  iVar1 = lua_type(param_1,2);
  if (iVar1 != 0) {
    lua_pushvalue(param_1,2);
    lua_pushvalue(param_1,param_2 + -1);
    lua_pushvalue(param_1,param_3 + -2);
    lua_call(param_1,2,1);
    uVar2 = lua_toboolean(param_1,0xffffffff);
    lua_settop(param_1,0xfffffffe);
    return (ulong)uVar2;
  }
  uVar3 = lua_lessthan(param_1,param_2,param_3);
  return uVar3;
}



// ======== luaT_init @ 00646e60 ========

void luaT_init(long param_1)

{
  byte *pbVar1;
  undefined8 uVar2;
  
  uVar2 = luaS_newlstr(param_1,"__index",7);
  *(undefined8 *)(*(long *)(param_1 + 0x20) + 0xa8) = uVar2;
  pbVar1 = (byte *)(*(long *)(*(long *)(param_1 + 0x20) + 0xa8) + 9);
  *pbVar1 = *pbVar1 | 0x10;
  uVar2 = luaS_newlstr(param_1,"__newindex",10);
  *(undefined8 *)(*(long *)(param_1 + 0x20) + 0xb0) = uVar2;
  pbVar1 = (byte *)(*(long *)(*(long *)(param_1 + 0x20) + 0xb0) + 9);
  *pbVar1 = *pbVar1 | 0x10;
  uVar2 = luaS_newlstr(param_1,&DAT_0065002d,4);
  *(undefined8 *)(*(long *)(param_1 + 0x20) + 0xb8) = uVar2;
  pbVar1 = (byte *)(*(long *)(*(long *)(param_1 + 0x20) + 0xb8) + 9);
  *pbVar1 = *pbVar1 | 0x10;
  uVar2 = luaS_newlstr(param_1,"__mode",6);
  *(undefined8 *)(*(long *)(param_1 + 0x20) + 0xc0) = uVar2;
  pbVar1 = (byte *)(*(long *)(*(long *)(param_1 + 0x20) + 0xc0) + 9);
  *pbVar1 = *pbVar1 | 0x10;
  uVar2 = luaS_newlstr(param_1,&DAT_00650065,4);
  *(undefined8 *)(*(long *)(param_1 + 0x20) + 200) = uVar2;
  pbVar1 = (byte *)(*(long *)(*(long *)(param_1 + 0x20) + 200) + 9);
  *pbVar1 = *pbVar1 | 0x10;
  uVar2 = luaS_newlstr(param_1,"__add",5);
  *(undefined8 *)(*(long *)(param_1 + 0x20) + 0xd0) = uVar2;
  pbVar1 = (byte *)(*(long *)(*(long *)(param_1 + 0x20) + 0xd0) + 9);
  *pbVar1 = *pbVar1 | 0x10;
  uVar2 = luaS_newlstr(param_1,"__sub",5);
  *(undefined8 *)(*(long *)(param_1 + 0x20) + 0xd8) = uVar2;
  pbVar1 = (byte *)(*(long *)(*(long *)(param_1 + 0x20) + 0xd8) + 9);
  *pbVar1 = *pbVar1 | 0x10;
  uVar2 = luaS_newlstr(param_1,"__mul",5);
  *(undefined8 *)(*(long *)(param_1 + 0x20) + 0xe0) = uVar2;
  pbVar1 = (byte *)(*(long *)(*(long *)(param_1 + 0x20) + 0xe0) + 9);
  *pbVar1 = *pbVar1 | 0x10;
  uVar2 = luaS_newlstr(param_1,&DAT_0065004f,5);
  *(undefined8 *)(*(long *)(param_1 + 0x20) + 0xe8) = uVar2;
  pbVar1 = (byte *)(*(long *)(*(long *)(param_1 + 0x20) + 0xe8) + 9);
  *pbVar1 = *pbVar1 | 0x10;
  uVar2 = luaS_newlstr(param_1,"__pow",5);
  *(undefined8 *)(*(long *)(param_1 + 0x20) + 0xf0) = uVar2;
  pbVar1 = (byte *)(*(long *)(*(long *)(param_1 + 0x20) + 0xf0) + 9);
  *pbVar1 = *pbVar1 | 0x10;
  uVar2 = luaS_newlstr(param_1,"__unm",5);
  *(undefined8 *)(*(long *)(param_1 + 0x20) + 0xf8) = uVar2;
  pbVar1 = (byte *)(*(long *)(*(long *)(param_1 + 0x20) + 0xf8) + 9);
  *pbVar1 = *pbVar1 | 0x10;
  uVar2 = luaS_newlstr(param_1,&DAT_0065005b,4);
  *(undefined8 *)(*(long *)(param_1 + 0x20) + 0x100) = uVar2;
  pbVar1 = (byte *)(*(long *)(*(long *)(param_1 + 0x20) + 0x100) + 9);
  *pbVar1 = *pbVar1 | 0x10;
  uVar2 = luaS_newlstr(param_1,&DAT_00650060,4);
  *(undefined8 *)(*(long *)(param_1 + 0x20) + 0x108) = uVar2;
  pbVar1 = (byte *)(*(long *)(*(long *)(param_1 + 0x20) + 0x108) + 9);
  *pbVar1 = *pbVar1 | 0x10;
  uVar2 = luaS_newlstr(param_1,"__concat",8);
  *(undefined8 *)(*(long *)(param_1 + 0x20) + 0x110) = uVar2;
  pbVar1 = (byte *)(*(long *)(*(long *)(param_1 + 0x20) + 0x110) + 9);
  *pbVar1 = *pbVar1 | 0x10;
  uVar2 = luaS_newlstr(param_1,"__call",6);
  *(undefined8 *)(*(long *)(param_1 + 0x20) + 0x118) = uVar2;
  pbVar1 = (byte *)(*(long *)(*(long *)(param_1 + 0x20) + 0x118) + 9);
  *pbVar1 = *pbVar1 | 0x10;
  return;
}



// ======== luaT_gettm @ 00647100 ========

int * luaT_gettm(long param_1,byte param_2,undefined8 param_3)

{
  int *piVar1;
  
  piVar1 = (int *)luaH_getstr(param_1,param_3);
  if (*piVar1 != 0) {
    return piVar1;
  }
  *(byte *)(param_1 + 10) = *(byte *)(param_1 + 10) | (byte)(1 << (param_2 & 0x1f));
  return (int *)0x0;
}



// ======== luaT_gettmbyobj @ 00647140 ========

undefined1 * luaT_gettmbyobj(long param_1,int *param_2,uint param_3)

{
  undefined1 *puVar1;
  
  if ((*param_2 != 7) && (*param_2 != 5)) {
    return luaO_nilobject;
  }
  puVar1 = (undefined1 *)
           luaH_getstr(*(undefined8 *)(*(long *)(param_2 + 2) + 0x10),
                       *(undefined8 *)(*(long *)(param_1 + 0x20) + 0xa8 + (ulong)param_3 * 8));
  return puVar1;
}



// ======== luaU_undump @ 00647180 ========

void luaU_undump(undefined8 param_1,long *param_2,undefined8 param_3)

{
  long lVar1;
  char cVar2;
  byte bVar3;
  byte *pbVar4;
  undefined1 *puVar5;
  double dVar6;
  int iVar7;
  uint uVar8;
  uint uVar9;
  uint uVar10;
  uint uVar11;
  char *pcVar12;
  char *pcVar13;
  undefined1 local_50;
  undefined1 uStack_4f;
  undefined1 uStack_4e;
  undefined1 uStack_4d;
  undefined1 uStack_4c;
  undefined1 uStack_4b;
  undefined1 uStack_4a;
  undefined1 uStack_49;
  undefined8 local_48;
  long *local_40;
  undefined8 local_38;
  uint local_30;
  char *local_28;
  
  pcVar13 = (char *)param_2[4];
  cVar2 = *pcVar13;
  if (cVar2 == '\x1b') {
    pcVar13 = "binary string";
  }
  else if ((cVar2 == '@') || (cVar2 == '=')) {
    pcVar13 = pcVar13 + 1;
  }
  lVar1 = *param_2;
  *param_2 = *param_2 + -1;
  local_48 = param_1;
  local_40 = param_2;
  local_38 = param_3;
  local_28 = pcVar13;
  if (lVar1 == 0) {
    iVar7 = luaZ_fill(param_2);
    if (iVar7 == -1) {
LAB_00647273:
      luaG_runerror(param_1,"unexpected end of file in %s",pcVar13);
    }
    else if (iVar7 == 0x1b) goto LAB_006471fc;
LAB_00647285:
    luaG_runerror(param_1,"bad signature in %s",pcVar13);
  }
  else {
    pcVar12 = (char *)param_2[1];
    param_2[1] = (long)(pcVar12 + 1);
    if (*pcVar12 != '\x1b') goto LAB_00647285;
LAB_006471fc:
    lVar1 = *param_2;
    *param_2 = *param_2 + -1;
    if (lVar1 == 0) {
      iVar7 = luaZ_fill(param_2);
      if (iVar7 == -1) goto LAB_00647273;
      if (iVar7 == 0x4c) goto LAB_0064722a;
      goto LAB_00647285;
    }
    pcVar12 = (char *)param_2[1];
    param_2[1] = (long)(pcVar12 + 1);
    if (*pcVar12 != 'L') goto LAB_00647285;
LAB_0064722a:
    lVar1 = *param_2;
    *param_2 = *param_2 + -1;
    if (lVar1 == 0) {
      iVar7 = luaZ_fill(param_2);
      if (iVar7 == -1) goto LAB_00647273;
      if (iVar7 == 0x75) goto LAB_00647258;
      goto LAB_00647285;
    }
    pcVar12 = (char *)param_2[1];
    param_2[1] = (long)(pcVar12 + 1);
    if (*pcVar12 != 'u') goto LAB_00647285;
LAB_00647258:
    lVar1 = *param_2;
    *param_2 = *param_2 + -1;
    if (lVar1 == 0) {
      uVar9 = luaZ_fill(param_2);
      if (uVar9 == 0xffffffff) goto LAB_00647273;
    }
    else {
      pbVar4 = (byte *)param_2[1];
      param_2[1] = (long)(pbVar4 + 1);
      uVar9 = (uint)*pbVar4;
    }
    if (uVar9 != 0x61) goto LAB_00647285;
  }
  lVar1 = *param_2;
  *param_2 = *param_2 + -1;
  if (lVar1 == 0) {
    uVar8 = luaZ_fill(param_2);
    if (uVar8 == 0xffffffff) {
      luaG_runerror(param_1,"unexpected end of file in %s",pcVar13);
      pcVar12 = "%s too new: read version %d.%d; expected at most %d.%d";
      uVar8 = 0xffffffff;
      uVar11 = 0xff;
    }
    else {
      uVar11 = uVar8 & 0xff;
      pcVar12 = "%s too new: read version %d.%d; expected at most %d.%d";
      uVar9 = uVar11;
      uVar10 = uVar8;
      if (uVar11 < 0x51) goto LAB_006472db;
    }
LAB_00647330:
    luaG_runerror(param_1,pcVar12,pcVar13,uVar11 >> 4,uVar8 & 0xf,5,0);
    lVar1 = *param_2;
    *param_2 = *param_2 + -1;
    if (lVar1 == 0) goto LAB_006472e8;
LAB_00647357:
    pbVar4 = (byte *)param_2[1];
    param_2[1] = (long)(pbVar4 + 1);
    uVar9 = (uint)*pbVar4;
  }
  else {
    pbVar4 = (byte *)param_2[1];
    param_2[1] = (long)(pbVar4 + 1);
    bVar3 = *pbVar4;
    uVar8 = (uint)bVar3;
    uVar11 = (uint)bVar3;
    pcVar12 = "%s too new: read version %d.%d; expected at most %d.%d";
    uVar9 = (uint)bVar3;
    uVar10 = (uint)bVar3;
    if (0x50 < bVar3) goto LAB_00647330;
LAB_006472db:
    uVar8 = uVar10;
    uVar11 = uVar9;
    pcVar12 = "%s too old: read version %d.%d; expected at least %d.%d";
    if (uVar11 != 0x50) goto LAB_00647330;
    lVar1 = *param_2;
    *param_2 = *param_2 + -1;
    if (lVar1 != 0) goto LAB_00647357;
LAB_006472e8:
    uVar9 = luaZ_fill(param_2);
    if (uVar9 == 0xffffffff) {
      luaG_runerror(param_1,"unexpected end of file in %s",pcVar13);
      uVar9 = 0xffffffff;
    }
  }
  local_30 = (uint)((uVar9 & 0xff) != 1);
  lVar1 = *param_2;
  *param_2 = *param_2 + -1;
  if (lVar1 == 0) {
    uVar10 = luaZ_fill(param_2);
    if (uVar10 != 0xffffffff) goto LAB_006473b1;
    luaG_runerror(param_1,"unexpected end of file in %s",pcVar13);
    uVar10 = 0xff;
LAB_006473bb:
    luaG_runerror(param_1,"virtual machine mismatch in %s: size of %s is %d but read %d",pcVar13,
                  "int",4,uVar10);
  }
  else {
    pbVar4 = (byte *)param_2[1];
    param_2[1] = (long)(pbVar4 + 1);
    uVar10 = (uint)*pbVar4;
LAB_006473b1:
    uVar10 = uVar10 & 0xff;
    if (uVar10 != 4) goto LAB_006473bb;
  }
  lVar1 = *param_2;
  *param_2 = *param_2 + -1;
  if (lVar1 == 0) {
    uVar10 = luaZ_fill(param_2);
    if (uVar10 != 0xffffffff) goto LAB_00647414;
    luaG_runerror(param_1,"unexpected end of file in %s",pcVar13);
    uVar10 = 0xff;
LAB_0064741e:
    luaG_runerror(param_1,"virtual machine mismatch in %s: size of %s is %d but read %d",pcVar13,
                  "LUAC_STR_SIZE_TYPE",4,uVar10);
  }
  else {
    pbVar4 = (byte *)param_2[1];
    param_2[1] = (long)(pbVar4 + 1);
    uVar10 = (uint)*pbVar4;
LAB_00647414:
    uVar10 = uVar10 & 0xff;
    if (uVar10 != 4) goto LAB_0064741e;
  }
  lVar1 = *param_2;
  *param_2 = *param_2 + -1;
  if (lVar1 == 0) {
    uVar10 = luaZ_fill(param_2);
    if (uVar10 != 0xffffffff) goto LAB_00647477;
    luaG_runerror(param_1,"unexpected end of file in %s",pcVar13);
    uVar10 = 0xff;
LAB_00647481:
    luaG_runerror(param_1,"virtual machine mismatch in %s: size of %s is %d but read %d",pcVar13,
                  "Instruction",4,uVar10);
  }
  else {
    pbVar4 = (byte *)param_2[1];
    param_2[1] = (long)(pbVar4 + 1);
    uVar10 = (uint)*pbVar4;
LAB_00647477:
    uVar10 = uVar10 & 0xff;
    if (uVar10 != 4) goto LAB_00647481;
  }
  lVar1 = *param_2;
  *param_2 = *param_2 + -1;
  if (lVar1 == 0) {
    uVar10 = luaZ_fill(param_2);
    if (uVar10 != 0xffffffff) goto LAB_006474da;
    luaG_runerror(param_1,"unexpected end of file in %s",pcVar13);
    uVar10 = 0xff;
LAB_006474e4:
    luaG_runerror(param_1,"virtual machine mismatch in %s: size of %s is %d but read %d",pcVar13,
                  &DAT_0066c084,6,uVar10);
  }
  else {
    pbVar4 = (byte *)param_2[1];
    param_2[1] = (long)(pbVar4 + 1);
    uVar10 = (uint)*pbVar4;
LAB_006474da:
    uVar10 = uVar10 & 0xff;
    if (uVar10 != 6) goto LAB_006474e4;
  }
  lVar1 = *param_2;
  *param_2 = *param_2 + -1;
  if (lVar1 == 0) {
    uVar10 = luaZ_fill(param_2);
    if (uVar10 != 0xffffffff) goto LAB_0064753d;
    luaG_runerror(param_1,"unexpected end of file in %s",pcVar13);
    uVar10 = 0xff;
LAB_00647547:
    luaG_runerror(param_1,"virtual machine mismatch in %s: size of %s is %d but read %d",pcVar13,
                  &DAT_0065b682,8,uVar10);
  }
  else {
    pbVar4 = (byte *)param_2[1];
    param_2[1] = (long)(pbVar4 + 1);
    uVar10 = (uint)*pbVar4;
LAB_0064753d:
    uVar10 = uVar10 & 0xff;
    if (uVar10 != 8) goto LAB_00647547;
  }
  lVar1 = *param_2;
  *param_2 = *param_2 + -1;
  if (lVar1 == 0) {
    uVar10 = luaZ_fill(param_2);
    if (uVar10 != 0xffffffff) goto LAB_006475a0;
    luaG_runerror(param_1,"unexpected end of file in %s",pcVar13);
    uVar10 = 0xff;
LAB_006475aa:
    luaG_runerror(param_1,"virtual machine mismatch in %s: size of %s is %d but read %d",pcVar13,
                  &DAT_0065a63d,9,uVar10);
  }
  else {
    pbVar4 = (byte *)param_2[1];
    param_2[1] = (long)(pbVar4 + 1);
    uVar10 = (uint)*pbVar4;
LAB_006475a0:
    uVar10 = uVar10 & 0xff;
    if (uVar10 != 9) goto LAB_006475aa;
  }
  lVar1 = *param_2;
  *param_2 = *param_2 + -1;
  if (lVar1 == 0) {
    uVar10 = luaZ_fill(param_2);
    if (uVar10 != 0xffffffff) goto LAB_00647603;
    luaG_runerror(param_1,"unexpected end of file in %s",pcVar13);
    uVar10 = 0xff;
LAB_0064760d:
    luaG_runerror(param_1,"virtual machine mismatch in %s: size of %s is %d but read %d",pcVar13,
                  &DAT_00656da5,9,uVar10);
  }
  else {
    pbVar4 = (byte *)param_2[1];
    param_2[1] = (long)(pbVar4 + 1);
    uVar10 = (uint)*pbVar4;
LAB_00647603:
    uVar10 = uVar10 & 0xff;
    if (uVar10 != 9) goto LAB_0064760d;
  }
  lVar1 = *param_2;
  *param_2 = *param_2 + -1;
  if (lVar1 == 0) {
    uVar10 = luaZ_fill(param_2);
    if (uVar10 != 0xffffffff) goto LAB_00647666;
    luaG_runerror(param_1,"unexpected end of file in %s",pcVar13);
    uVar10 = 0xff;
LAB_00647670:
    luaG_runerror(param_1,"virtual machine mismatch in %s: size of %s is %d but read %d",pcVar13,
                  "number",8,uVar10);
  }
  else {
    pbVar4 = (byte *)param_2[1];
    param_2[1] = (long)(pbVar4 + 1);
    uVar10 = (uint)*pbVar4;
LAB_00647666:
    uVar10 = uVar10 & 0xff;
    if (uVar10 != 8) goto LAB_00647670;
  }
  if ((uVar9 & 0xff) == 1) {
    iVar7 = luaZ_read(param_2,&local_50,8);
    if (iVar7 != 0) {
      luaG_runerror(param_1,"unexpected end of file in %s",pcVar13);
    }
    dVar6 = (double)CONCAT17(uStack_49,
                             CONCAT16(uStack_4a,
                                      CONCAT15(uStack_4b,
                                               CONCAT14(uStack_4c,
                                                        CONCAT13(uStack_4d,
                                                                 CONCAT12(uStack_4e,
                                                                          CONCAT11(uStack_4f,
                                                                                   local_50)))))));
    goto joined_r0x00647982;
  }
  lVar1 = *param_2;
  *param_2 = *param_2 + -1;
  if (lVar1 == 0) {
    iVar7 = luaZ_fill(param_2);
    if (iVar7 == -1) {
      luaG_runerror(param_1,"unexpected end of file in %s",pcVar13);
      iVar7 = -1;
    }
    uStack_49 = (undefined1)iVar7;
    lVar1 = *param_2;
    *param_2 = *param_2 + -1;
    if (lVar1 != 0) goto LAB_006478b4;
LAB_006476d2:
    iVar7 = luaZ_fill(param_2);
    if (iVar7 == -1) {
      luaG_runerror(param_1,"unexpected end of file in %s",pcVar13);
      iVar7 = -1;
    }
    uStack_4a = (undefined1)iVar7;
    lVar1 = *param_2;
    *param_2 = *param_2 + -1;
    if (lVar1 != 0) goto LAB_006478d1;
LAB_00647704:
    iVar7 = luaZ_fill(param_2);
    if (iVar7 == -1) {
      luaG_runerror(param_1,"unexpected end of file in %s",pcVar13);
      iVar7 = -1;
    }
    uStack_4b = (undefined1)iVar7;
    lVar1 = *param_2;
    *param_2 = *param_2 + -1;
    if (lVar1 != 0) goto LAB_006478ee;
LAB_00647736:
    iVar7 = luaZ_fill(param_2);
    if (iVar7 == -1) {
      luaG_runerror(param_1,"unexpected end of file in %s",pcVar13);
      iVar7 = -1;
    }
    uStack_4c = (undefined1)iVar7;
    lVar1 = *param_2;
    *param_2 = *param_2 + -1;
    if (lVar1 != 0) goto LAB_0064790b;
LAB_00647768:
    iVar7 = luaZ_fill(param_2);
    if (iVar7 == -1) {
      luaG_runerror(param_1,"unexpected end of file in %s",pcVar13);
      iVar7 = -1;
    }
    uStack_4d = (undefined1)iVar7;
    lVar1 = *param_2;
    *param_2 = *param_2 + -1;
    if (lVar1 != 0) goto LAB_00647928;
LAB_0064779a:
    iVar7 = luaZ_fill(param_2);
    if (iVar7 == -1) {
      luaG_runerror(param_1,"unexpected end of file in %s",pcVar13);
      iVar7 = -1;
    }
    uStack_4e = (undefined1)iVar7;
    lVar1 = *param_2;
    *param_2 = *param_2 + -1;
    if (lVar1 != 0) goto LAB_00647945;
LAB_006477cc:
    iVar7 = luaZ_fill(param_2);
    if (iVar7 == -1) {
      luaG_runerror(param_1,"unexpected end of file in %s",pcVar13);
      iVar7 = -1;
    }
    uStack_4f = (undefined1)iVar7;
    lVar1 = *param_2;
    *param_2 = *param_2 + -1;
  }
  else {
    puVar5 = (undefined1 *)param_2[1];
    param_2[1] = (long)(puVar5 + 1);
    uStack_49 = *puVar5;
    lVar1 = *param_2;
    *param_2 = *param_2 + -1;
    if (lVar1 == 0) goto LAB_006476d2;
LAB_006478b4:
    puVar5 = (undefined1 *)param_2[1];
    param_2[1] = (long)(puVar5 + 1);
    uStack_4a = *puVar5;
    lVar1 = *param_2;
    *param_2 = *param_2 + -1;
    if (lVar1 == 0) goto LAB_00647704;
LAB_006478d1:
    puVar5 = (undefined1 *)param_2[1];
    param_2[1] = (long)(puVar5 + 1);
    uStack_4b = *puVar5;
    lVar1 = *param_2;
    *param_2 = *param_2 + -1;
    if (lVar1 == 0) goto LAB_00647736;
LAB_006478ee:
    puVar5 = (undefined1 *)param_2[1];
    param_2[1] = (long)(puVar5 + 1);
    uStack_4c = *puVar5;
    lVar1 = *param_2;
    *param_2 = *param_2 + -1;
    if (lVar1 == 0) goto LAB_00647768;
LAB_0064790b:
    puVar5 = (undefined1 *)param_2[1];
    param_2[1] = (long)(puVar5 + 1);
    uStack_4d = *puVar5;
    lVar1 = *param_2;
    *param_2 = *param_2 + -1;
    if (lVar1 == 0) goto LAB_0064779a;
LAB_00647928:
    puVar5 = (undefined1 *)param_2[1];
    param_2[1] = (long)(puVar5 + 1);
    uStack_4e = *puVar5;
    lVar1 = *param_2;
    *param_2 = *param_2 + -1;
    if (lVar1 == 0) goto LAB_006477cc;
LAB_00647945:
    puVar5 = (undefined1 *)param_2[1];
    param_2[1] = (long)(puVar5 + 1);
    uStack_4f = *puVar5;
    lVar1 = *param_2;
    *param_2 = *param_2 + -1;
  }
  if (lVar1 == 0) {
    iVar7 = luaZ_fill(param_2);
    if (iVar7 == -1) {
      luaG_runerror(param_1,"unexpected end of file in %s",pcVar13);
      iVar7 = -1;
    }
    local_50 = (undefined1)iVar7;
    dVar6 = (double)CONCAT17(uStack_49,
                             CONCAT16(uStack_4a,
                                      CONCAT15(uStack_4b,
                                               CONCAT14(uStack_4c,
                                                        CONCAT13(uStack_4d,
                                                                 CONCAT12(uStack_4e,
                                                                          CONCAT11(uStack_4f,
                                                                                   local_50)))))));
  }
  else {
    puVar5 = (undefined1 *)param_2[1];
    param_2[1] = (long)(puVar5 + 1);
    local_50 = *puVar5;
    dVar6 = (double)CONCAT17(uStack_49,
                             CONCAT16(uStack_4a,
                                      CONCAT15(uStack_4b,
                                               CONCAT14(uStack_4c,
                                                        CONCAT13(uStack_4d,
                                                                 CONCAT12(uStack_4e,
                                                                          CONCAT11(uStack_4f,
                                                                                   local_50)))))));
  }
joined_r0x00647982:
  if ((long)dVar6 != 0x1df5e76) {
    luaG_runerror(param_1,"unknown number format in %s",pcVar13);
  }
  LoadFunction(&local_48,0);
  return;
}



// ======== luaU_endianness @ 006479b0 ========

undefined8 luaU_endianness(void)

{
  return 1;
}



// ======== LoadFunction @ 006479c0 ========

/* WARNING: Type propagation algorithm not settling */

long LoadFunction(undefined8 *param_1,long param_2)

{
  long lVar1;
  undefined1 *puVar2;
  byte *pbVar3;
  char cVar4;
  int iVar5;
  uint uVar6;
  undefined8 in_RAX;
  long lVar7;
  long lVar8;
  undefined8 uVar9;
  ulong uVar10;
  long lVar11;
  long *plVar12;
  long lVar13;
  undefined8 local_38;
  
  local_38 = in_RAX;
  lVar7 = luaF_newproto(*param_1);
  lVar8 = LoadString(param_1);
  if (lVar8 != 0) {
    param_2 = lVar8;
  }
  *(long *)(lVar7 + 0x40) = param_2;
  LoadBlock(param_1,&local_38,4);
  if ((int)(uint)local_38 < 0) {
    luaG_runerror(*param_1,"bad integer in %s",param_1[4]);
    *(uint *)(lVar7 + 0x60) = (uint)local_38;
    plVar12 = (long *)param_1[1];
    lVar8 = *plVar12;
    *plVar12 = *plVar12 + -1;
  }
  else {
    *(uint *)(lVar7 + 0x60) = (uint)local_38;
    plVar12 = (long *)param_1[1];
    lVar8 = *plVar12;
    *plVar12 = *plVar12 + -1;
  }
  if (lVar8 == 0) {
    iVar5 = luaZ_fill();
    if (iVar5 == -1) {
      luaG_runerror(*param_1,"unexpected end of file in %s",param_1[4]);
      iVar5 = -1;
    }
    *(char *)(lVar7 + 0x70) = (char)iVar5;
    plVar12 = (long *)param_1[1];
    lVar8 = *plVar12;
    *plVar12 = *plVar12 + -1;
  }
  else {
    puVar2 = *(undefined1 **)((long)plVar12 + 8);
    *(undefined1 **)((long)plVar12 + 8) = puVar2 + 1;
    *(undefined1 *)(lVar7 + 0x70) = *puVar2;
    plVar12 = (long *)param_1[1];
    lVar8 = *plVar12;
    *plVar12 = *plVar12 + -1;
  }
  if (lVar8 == 0) {
    iVar5 = luaZ_fill();
    if (iVar5 == -1) {
      luaG_runerror(*param_1,"unexpected end of file in %s",param_1[4]);
      iVar5 = -1;
    }
    *(char *)(lVar7 + 0x71) = (char)iVar5;
    plVar12 = (long *)param_1[1];
    lVar8 = *plVar12;
    *plVar12 = *plVar12 + -1;
  }
  else {
    puVar2 = *(undefined1 **)((long)plVar12 + 8);
    *(undefined1 **)((long)plVar12 + 8) = puVar2 + 1;
    *(undefined1 *)(lVar7 + 0x71) = *puVar2;
    plVar12 = (long *)param_1[1];
    lVar8 = *plVar12;
    *plVar12 = *plVar12 + -1;
  }
  if (lVar8 == 0) {
    iVar5 = luaZ_fill();
    if (iVar5 == -1) {
      luaG_runerror(*param_1,"unexpected end of file in %s",param_1[4]);
      iVar5 = -1;
    }
    *(char *)(lVar7 + 0x72) = (char)iVar5;
    plVar12 = (long *)param_1[1];
    lVar8 = *plVar12;
    *plVar12 = *plVar12 + -1;
  }
  else {
    puVar2 = (undefined1 *)plVar12[1];
    plVar12[1] = (long)(puVar2 + 1);
    *(undefined1 *)(lVar7 + 0x72) = *puVar2;
    plVar12 = (long *)param_1[1];
    lVar8 = *plVar12;
    *plVar12 = *plVar12 + -1;
  }
  if (lVar8 == 0) {
    uVar6 = luaZ_fill();
    if (uVar6 == 0xffffffff) {
      luaG_runerror(*param_1,"unexpected end of file in %s",param_1[4]);
      uVar6 = 0xffffffff;
    }
  }
  else {
    pbVar3 = (byte *)plVar12[1];
    plVar12[1] = (long)(pbVar3 + 1);
    uVar6 = (uint)*pbVar3;
  }
  *(char *)(lVar7 + 0x73) = (char)uVar6;
  LoadBlock(param_1,&local_38,4);
  iVar5 = (uint)local_38;
  if ((int)(uint)local_38 < 0) {
    luaG_runerror(*param_1,"bad integer in %s",param_1[4]);
    iVar5 = (uint)local_38;
  }
  uVar9 = luaM_realloc(*param_1,0,0,(long)iVar5 * 4);
  *(undefined8 *)(lVar7 + 0x28) = uVar9;
  *(int *)(lVar7 + 0x54) = iVar5;
  LoadVector(param_1,uVar9,iVar5);
  LoadBlock(param_1,&local_38,4);
  uVar6 = (uint)local_38;
  if ((int)(uint)local_38 < 0) {
    luaG_runerror(*param_1,"bad integer in %s",param_1[4]);
    uVar6 = (uint)local_38;
  }
  uVar9 = luaM_realloc(*param_1,0,0,(long)(int)uVar6 << 4);
  *(undefined8 *)(lVar7 + 0x30) = uVar9;
  *(uint *)(lVar7 + 0x5c) = uVar6;
  if (0 < (int)uVar6) {
    lVar8 = 0;
    do {
      uVar9 = LoadString(param_1);
      *(undefined8 *)(*(long *)(lVar7 + 0x30) + lVar8) = uVar9;
      LoadBlock(param_1,&local_38,4);
      if ((int)(uint)local_38 < 0) {
        luaG_runerror(*param_1,"bad integer in %s",param_1[4]);
      }
      *(uint *)(*(long *)(lVar7 + 0x30) + 8 + lVar8) = (uint)local_38;
      LoadBlock(param_1,&local_38,4);
      if ((int)(uint)local_38 < 0) {
        luaG_runerror(*param_1,"bad integer in %s",param_1[4]);
      }
      *(uint *)(*(long *)(lVar7 + 0x30) + 0xc + lVar8) = (uint)local_38;
      lVar8 = lVar8 + 0x10;
    } while ((ulong)uVar6 << 4 != lVar8);
  }
  LoadBlock(param_1,&local_38,4);
  uVar6 = (uint)local_38;
  if ((int)(uint)local_38 < 0) {
    luaG_runerror(*param_1,"bad integer in %s",param_1[4]);
    uVar6 = (uint)local_38;
  }
  if ((uVar6 != 0) && (uVar6 != *(byte *)(lVar7 + 0x70))) {
    luaG_runerror(*param_1,"bad nupvalues in %s: read %d; expected %d",param_1[4],uVar6);
  }
  uVar9 = luaM_realloc(*param_1,0,0,(long)(int)uVar6 << 3);
  *(undefined8 *)(lVar7 + 0x38) = uVar9;
  *(uint *)(lVar7 + 0x48) = uVar6;
  if (0 < (int)uVar6) {
    uVar10 = 0;
    do {
      uVar9 = LoadString(param_1);
      *(undefined8 *)(*(long *)(lVar7 + 0x38) + uVar10 * 8) = uVar9;
      uVar10 = uVar10 + 1;
    } while (uVar6 != uVar10);
  }
  LoadBlock(param_1,&local_38,4);
  uVar6 = (uint)local_38;
  if ((int)(uint)local_38 < 0) {
    luaG_runerror(*param_1,"bad integer in %s",param_1[4]);
    uVar6 = (uint)local_38;
  }
  lVar8 = luaM_realloc(*param_1,0,0,(long)(int)uVar6 << 4);
  *(long *)(lVar7 + 0x10) = lVar8;
  *(uint *)(lVar7 + 0x4c) = uVar6;
  if (0 < (int)uVar6) {
    lVar13 = (ulong)uVar6 * 0x10 + -0x10;
    lVar11 = 0;
    plVar12 = (long *)param_1[1];
    lVar1 = *plVar12;
    *plVar12 = *plVar12 + -1;
    if (lVar1 != 0) goto LAB_00647e30;
LAB_00647e00:
    uVar6 = luaZ_fill();
    if (uVar6 != 0xffffffff) goto LAB_00647e3f;
    luaG_runerror(*param_1,"unexpected end of file in %s",param_1[4]);
    uVar6 = 0xffffffff;
LAB_00647e80:
    luaG_runerror(*param_1,"bad constant type (%d) in %s",uVar6 & 0xff,param_1[4]);
    if (lVar13 != lVar11) {
      do {
        lVar8 = *(long *)(lVar7 + 0x10);
        lVar11 = lVar11 + 0x10;
        plVar12 = (long *)param_1[1];
        lVar1 = *plVar12;
        *plVar12 = *plVar12 + -1;
        if (lVar1 == 0) goto LAB_00647e00;
LAB_00647e30:
        pbVar3 = (byte *)plVar12[1];
        plVar12[1] = (long)(pbVar3 + 1);
        uVar6 = (uint)*pbVar3;
LAB_00647e3f:
        cVar4 = (char)uVar6;
        if (cVar4 == '\0') {
          *(undefined4 *)(lVar8 + lVar11) = 0;
        }
        else if (cVar4 == '\x04') {
          *(undefined4 *)(lVar8 + lVar11) = 4;
          uVar9 = LoadString(param_1);
          *(undefined8 *)(lVar8 + 8 + lVar11) = uVar9;
        }
        else {
          if (cVar4 != '\x03') goto LAB_00647e80;
          *(undefined4 *)(lVar8 + lVar11) = 3;
          LoadBlock(param_1,&local_38,8);
          *(undefined8 *)(lVar8 + 8 + lVar11) = local_38;
        }
        if (lVar13 == lVar11) break;
      } while( true );
    }
  }
  LoadBlock(param_1,&local_38,4);
  uVar6 = (uint)local_38;
  if ((int)(uint)local_38 < 0) {
    luaG_runerror(*param_1,"bad integer in %s",param_1[4]);
    uVar6 = (uint)local_38;
  }
  uVar9 = luaM_realloc(*param_1,0,0,(long)(int)uVar6 * 8);
  *(undefined8 *)(lVar7 + 0x20) = uVar9;
  *(uint *)(lVar7 + 0x58) = uVar6;
  if (0 < (int)uVar6) {
    uVar10 = 0;
    do {
      uVar9 = LoadFunction(param_1,*(undefined8 *)(lVar7 + 0x40));
      *(undefined8 *)(*(long *)(lVar7 + 0x20) + uVar10 * 8) = uVar9;
      uVar10 = uVar10 + 1;
    } while (uVar6 != uVar10);
  }
  LoadBlock(param_1,&local_38,4);
  iVar5 = (uint)local_38;
  if ((int)(uint)local_38 < 0) {
    luaG_runerror(*param_1,"bad integer in %s",param_1[4]);
    iVar5 = (uint)local_38;
  }
  uVar9 = luaM_realloc(*param_1,0,0,(long)iVar5 * 4);
  *(undefined8 *)(lVar7 + 0x18) = uVar9;
  *(int *)(lVar7 + 0x50) = iVar5;
  LoadVector(param_1,uVar9,iVar5);
  iVar5 = luaG_checkcode(lVar7);
  if (iVar5 == 0) {
    luaG_runerror(*param_1,"bad code in %s",param_1[4]);
  }
  return lVar7;
}



// ======== LoadBlock @ 00647ff0 ========

void LoadBlock(undefined8 *param_1,long param_2,uint param_3)

{
  long lVar1;
  byte *pbVar2;
  long *plVar3;
  uint uVar4;
  int iVar5;
  ulong uVar6;
  
  if (*(int *)(param_1 + 3) == 0) {
    iVar5 = luaZ_read(param_1[1],param_2,(long)(int)param_3);
    if (iVar5 != 0) {
      luaG_runerror(*param_1,"unexpected end of file in %s",param_1[4]);
      return;
    }
  }
  else if (param_3 != 0) {
    uVar6 = (ulong)param_3;
    do {
      plVar3 = (long *)param_1[1];
      lVar1 = *plVar3;
      *plVar3 = *plVar3 + -1;
      if (lVar1 == 0) {
        uVar4 = luaZ_fill();
        if (uVar4 == 0xffffffff) {
          luaG_runerror(*param_1,"unexpected end of file in %s",param_1[4]);
          uVar4 = 0xffffffff;
        }
      }
      else {
        pbVar2 = (byte *)plVar3[1];
        plVar3[1] = (long)(pbVar2 + 1);
        uVar4 = (uint)*pbVar2;
      }
      *(char *)(param_2 + -1 + uVar6) = (char)uVar4;
      uVar6 = uVar6 - 1;
    } while ((int)uVar6 != 0);
  }
  return;
}



// ======== LoadString @ 00648090 ========

undefined8 LoadString(undefined8 *param_1)

{
  int iVar1;
  undefined8 uVar2;
  long lVar3;
  int local_1c;
  
  LoadBlock(param_1,&local_1c,4);
  lVar3 = (long)local_1c;
  if (lVar3 == 0) {
    uVar2 = 0;
  }
  else {
    uVar2 = luaZ_openspace(*param_1,param_1[2],lVar3);
    iVar1 = luaZ_read(param_1[1],uVar2,lVar3);
    if (iVar1 != 0) {
      luaG_runerror(*param_1,"unexpected end of file in %s",param_1[4]);
    }
    uVar2 = luaS_newlstr(*param_1,uVar2,(long)(local_1c + -1));
  }
  return uVar2;
}



// ======== LoadVector @ 00648110 ========

void LoadVector(undefined8 *param_1,long param_2,int param_3)

{
  long lVar1;
  byte *pbVar2;
  undefined1 *puVar3;
  int iVar4;
  uint uVar5;
  long lVar6;
  long *plVar7;
  
  if (*(int *)(param_1 + 3) == 0) {
    iVar4 = luaZ_read(param_1[1],param_2,(long)(param_3 << 2));
    if (iVar4 != 0) {
      luaG_runerror(*param_1,"unexpected end of file in %s",param_1[4]);
      return;
    }
  }
  else if (param_3 != 0) {
    lVar6 = 0;
    do {
      plVar7 = (long *)param_1[1];
      lVar1 = *plVar7;
      *plVar7 = *plVar7 + -1;
      if (lVar1 == 0) {
        iVar4 = luaZ_fill();
        if (iVar4 == -1) {
          luaG_runerror(*param_1,"unexpected end of file in %s",param_1[4]);
          iVar4 = -1;
        }
        *(char *)(param_2 + 3 + lVar6 * 4) = (char)iVar4;
        plVar7 = (long *)param_1[1];
        lVar1 = *plVar7;
        *plVar7 = *plVar7 + -1;
        if (lVar1 == 0) goto LAB_006481b4;
LAB_00648243:
        puVar3 = (undefined1 *)plVar7[1];
        plVar7[1] = (long)(puVar3 + 1);
        *(undefined1 *)(param_2 + 2 + lVar6 * 4) = *puVar3;
        plVar7 = (long *)param_1[1];
        lVar1 = *plVar7;
        *plVar7 = *plVar7 + -1;
        if (lVar1 != 0) goto LAB_00648262;
LAB_006481f0:
        uVar5 = luaZ_fill();
        if (uVar5 == 0xffffffff) {
          luaG_runerror(*param_1,"unexpected end of file in %s",param_1[4]);
          uVar5 = 0xffffffff;
        }
      }
      else {
        puVar3 = (undefined1 *)plVar7[1];
        plVar7[1] = (long)(puVar3 + 1);
        *(undefined1 *)(param_2 + 3 + lVar6 * 4) = *puVar3;
        plVar7 = (long *)param_1[1];
        lVar1 = *plVar7;
        *plVar7 = *plVar7 + -1;
        if (lVar1 != 0) goto LAB_00648243;
LAB_006481b4:
        iVar4 = luaZ_fill();
        if (iVar4 == -1) {
          luaG_runerror(*param_1,"unexpected end of file in %s",param_1[4]);
          iVar4 = -1;
        }
        *(char *)(param_2 + 2 + lVar6 * 4) = (char)iVar4;
        plVar7 = (long *)param_1[1];
        lVar1 = *plVar7;
        *plVar7 = *plVar7 + -1;
        if (lVar1 == 0) goto LAB_006481f0;
LAB_00648262:
        pbVar2 = (byte *)plVar7[1];
        plVar7[1] = (long)(pbVar2 + 1);
        uVar5 = (uint)*pbVar2;
      }
      *(char *)(param_2 + 1 + lVar6 * 4) = (char)uVar5;
      plVar7 = (long *)param_1[1];
      lVar1 = *plVar7;
      *plVar7 = *plVar7 + -1;
      if (lVar1 == 0) {
        uVar5 = luaZ_fill();
        if (uVar5 == 0xffffffff) {
          luaG_runerror(*param_1,"unexpected end of file in %s",param_1[4]);
          uVar5 = 0xffffffff;
        }
      }
      else {
        pbVar2 = (byte *)plVar7[1];
        plVar7[1] = (long)(pbVar2 + 1);
        uVar5 = (uint)*pbVar2;
      }
      *(char *)(param_2 + lVar6 * 4) = (char)uVar5;
      lVar6 = lVar6 + 1;
    } while (param_3 != (int)lVar6);
  }
  return;
}



// ======== luaV_tonumber @ 00648300 ========

int * luaV_tonumber(int *param_1,int *param_2)

{
  int iVar1;
  undefined8 local_10;
  
  if (*param_1 == 3) {
    return param_1;
  }
  if (*param_1 == 4) {
    iVar1 = luaO_str2d(*(long *)(param_1 + 2) + 0x18,&local_10);
    if (iVar1 != 0) {
      *param_2 = 3;
      *(undefined8 *)(param_2 + 2) = local_10;
      return param_2;
    }
  }
  return (int *)0x0;
}



// ======== luaV_tostring @ 00648360 ========

bool luaV_tostring(undefined8 param_1,int *param_2)

{
  int iVar1;
  size_t sVar2;
  undefined8 uVar3;
  char acStack_38 [32];
  
  iVar1 = *param_2;
  if (iVar1 == 3) {
    sprintf(acStack_38,"%.14g",*(undefined8 *)(param_2 + 2));
    *param_2 = 4;
    sVar2 = strlen(acStack_38);
    uVar3 = luaS_newlstr(param_1,acStack_38,sVar2);
    *(undefined8 *)(param_2 + 2) = uVar3;
  }
  return iVar1 == 3;
}



// ======== luaV_gettable @ 006483c0 ========

int * luaV_gettable(long param_1,int *param_2,undefined4 *param_3,int param_4)

{
  int iVar1;
  undefined4 *puVar2;
  int *piVar3;
  long lVar4;
  
  do {
    if (100 < param_4) {
      luaG_runerror(param_1,"loop in gettable");
    }
    if (*param_2 == 5) {
      piVar3 = (int *)luaH_get(*(undefined8 *)(param_2 + 2),param_3);
      if (*piVar3 != 0) {
        return piVar3;
      }
      if ((*(byte *)(*(long *)(*(long *)(param_2 + 2) + 0x10) + 10) & 1) != 0) {
        return (int *)luaO_nilobject;
      }
      piVar3 = (int *)luaT_gettm(*(long *)(*(long *)(param_2 + 2) + 0x10),0,
                                 *(undefined8 *)(*(long *)(param_1 + 0x20) + 0xa8));
      if (piVar3 == (int *)0x0) {
        return (int *)luaO_nilobject;
      }
      iVar1 = *piVar3;
    }
    else {
      piVar3 = (int *)luaT_gettmbyobj(param_1,param_2,0);
      iVar1 = *piVar3;
      if (iVar1 == 0) {
        luaG_typeerror(param_1,param_2,"index");
        iVar1 = *piVar3;
      }
    }
    if (iVar1 == 6) {
      puVar2 = *(undefined4 **)(param_1 + 0x10);
      *puVar2 = 6;
      *(undefined8 *)(puVar2 + 2) = *(undefined8 *)(piVar3 + 2);
      lVar4 = *(long *)(param_1 + 0x10);
      *(int *)(lVar4 + 0x10) = *param_2;
      *(undefined8 *)(lVar4 + 0x18) = *(undefined8 *)(param_2 + 2);
      lVar4 = *(long *)(param_1 + 0x10);
      *(undefined4 *)(lVar4 + 0x20) = *param_3;
      *(undefined8 *)(lVar4 + 0x28) = *(undefined8 *)(param_3 + 2);
      lVar4 = *(long *)(param_1 + 0x10);
      if (*(long *)(param_1 + 0x30) - lVar4 < 0x31) {
        luaD_growstack(param_1,3);
        lVar4 = *(long *)(param_1 + 0x10);
      }
      *(long *)(param_1 + 0x10) = lVar4 + 0x30;
      luaD_call(param_1,lVar4,1);
      piVar3 = (int *)(*(long *)(param_1 + 0x10) + -0x10);
      *(int **)(param_1 + 0x10) = piVar3;
      return piVar3;
    }
    param_4 = param_4 + 1;
    param_2 = piVar3;
  } while( true );
}



// ======== luaV_index @ 00648520 ========

undefined1 * luaV_index(long param_1,undefined4 *param_2,undefined4 *param_3,undefined4 param_4)

{
  undefined4 *puVar1;
  int *piVar2;
  undefined1 *puVar3;
  long lVar4;
  
  puVar3 = luaO_nilobject;
  if ((*(byte *)(*(long *)(*(long *)(param_2 + 2) + 0x10) + 10) & 1) == 0) {
    piVar2 = (int *)luaT_gettm(*(long *)(*(long *)(param_2 + 2) + 0x10),0,
                               *(undefined8 *)(*(long *)(param_1 + 0x20) + 0xa8));
    if (piVar2 != (int *)0x0) {
      if (*piVar2 != 6) {
        puVar3 = (undefined1 *)luaV_gettable(param_1,piVar2,param_3,param_4);
        return puVar3;
      }
      puVar1 = *(undefined4 **)(param_1 + 0x10);
      *puVar1 = 6;
      *(undefined8 *)(puVar1 + 2) = *(undefined8 *)(piVar2 + 2);
      lVar4 = *(long *)(param_1 + 0x10);
      *(undefined4 *)(lVar4 + 0x10) = *param_2;
      *(undefined8 *)(lVar4 + 0x18) = *(undefined8 *)(param_2 + 2);
      lVar4 = *(long *)(param_1 + 0x10);
      *(undefined4 *)(lVar4 + 0x20) = *param_3;
      *(undefined8 *)(lVar4 + 0x28) = *(undefined8 *)(param_3 + 2);
      lVar4 = *(long *)(param_1 + 0x10);
      if (*(long *)(param_1 + 0x30) - lVar4 < 0x31) {
        luaD_growstack(param_1,3);
        lVar4 = *(long *)(param_1 + 0x10);
      }
      *(long *)(param_1 + 0x10) = lVar4 + 0x30;
      luaD_call(param_1,lVar4,1);
      puVar3 = (undefined1 *)(*(long *)(param_1 + 0x10) + -0x10);
      *(undefined1 **)(param_1 + 0x10) = puVar3;
    }
  }
  return puVar3;
}



// ======== luaV_getnotable @ 00648610 ========

void luaV_getnotable(long param_1,undefined4 *param_2,undefined4 *param_3,undefined4 param_4)

{
  int iVar1;
  undefined4 *puVar2;
  int *piVar3;
  long lVar4;
  
  piVar3 = (int *)luaT_gettmbyobj(param_1,param_2,0);
  iVar1 = *piVar3;
  if (iVar1 == 0) {
    luaG_typeerror(param_1,param_2,"index");
    iVar1 = *piVar3;
  }
  if (iVar1 != 6) {
    luaV_gettable(param_1,piVar3,param_3,param_4);
    return;
  }
  puVar2 = *(undefined4 **)(param_1 + 0x10);
  *puVar2 = 6;
  *(undefined8 *)(puVar2 + 2) = *(undefined8 *)(piVar3 + 2);
  lVar4 = *(long *)(param_1 + 0x10);
  *(undefined4 *)(lVar4 + 0x10) = *param_2;
  *(undefined8 *)(lVar4 + 0x18) = *(undefined8 *)(param_2 + 2);
  lVar4 = *(long *)(param_1 + 0x10);
  *(undefined4 *)(lVar4 + 0x20) = *param_3;
  *(undefined8 *)(lVar4 + 0x28) = *(undefined8 *)(param_3 + 2);
  lVar4 = *(long *)(param_1 + 0x10);
  if (*(long *)(param_1 + 0x30) - lVar4 < 0x31) {
    luaD_growstack(param_1,3);
    lVar4 = *(long *)(param_1 + 0x10);
  }
  *(long *)(param_1 + 0x10) = lVar4 + 0x30;
  luaD_call(param_1,lVar4,1);
  *(long *)(param_1 + 0x10) = *(long *)(param_1 + 0x10) + -0x10;
  return;
}



// ======== luaV_settable @ 00648700 ========

void luaV_settable(long param_1,int *param_2,undefined4 *param_3,int *param_4)

{
  undefined4 *puVar1;
  int iVar2;
  int *piVar3;
  int *piVar4;
  long lVar5;
  int iVar6;
  
  iVar2 = *param_2;
  iVar6 = 0x65;
  do {
    if (iVar2 == 5) {
      lVar5 = *(long *)(param_2 + 2);
      piVar3 = (int *)luaH_set(param_1,lVar5,param_3);
      if (((*piVar3 != 0) || (lVar5 = *(long *)(lVar5 + 0x10), (*(byte *)(lVar5 + 10) & 2) != 0)) ||
         (piVar4 = (int *)luaT_gettm(lVar5,1,*(undefined8 *)(*(long *)(param_1 + 0x20) + 0xb0)),
         piVar4 == (int *)0x0)) {
        *piVar3 = *param_4;
        *(undefined8 *)(piVar3 + 2) = *(undefined8 *)(param_4 + 2);
        return;
      }
    }
    else {
      piVar4 = (int *)luaT_gettmbyobj(param_1,param_2,1);
      if (*piVar4 == 0) {
        luaG_typeerror(param_1,param_2,"index");
      }
    }
    iVar2 = *piVar4;
    if (iVar2 == 6) {
      puVar1 = *(undefined4 **)(param_1 + 0x10);
      *puVar1 = 6;
      *(undefined8 *)(puVar1 + 2) = *(undefined8 *)(piVar4 + 2);
      lVar5 = *(long *)(param_1 + 0x10);
      *(int *)(lVar5 + 0x10) = *param_2;
      *(undefined8 *)(lVar5 + 0x18) = *(undefined8 *)(param_2 + 2);
      lVar5 = *(long *)(param_1 + 0x10);
      *(undefined4 *)(lVar5 + 0x20) = *param_3;
      *(undefined8 *)(lVar5 + 0x28) = *(undefined8 *)(param_3 + 2);
      lVar5 = *(long *)(param_1 + 0x10);
      *(int *)(lVar5 + 0x30) = *param_4;
      *(undefined8 *)(lVar5 + 0x38) = *(undefined8 *)(param_4 + 2);
      lVar5 = *(long *)(param_1 + 0x10);
      if (*(long *)(param_1 + 0x30) - lVar5 < 0x41) {
        luaD_growstack(param_1,4);
        lVar5 = *(long *)(param_1 + 0x10);
      }
      *(long *)(param_1 + 0x10) = lVar5 + 0x40;
      luaD_call(param_1,lVar5,0);
      return;
    }
    iVar6 = iVar6 + -1;
    param_2 = piVar4;
  } while (iVar6 != 0);
  luaG_runerror(param_1,"loop in settable");
  return;
}



// ======== luaV_lessthan @ 006488a0 ========

ulong luaV_lessthan(undefined8 param_1,int *param_2,int *param_3)

{
  int iVar1;
  long lVar2;
  long lVar3;
  uint uVar4;
  ulong uVar5;
  size_t sVar6;
  size_t sVar7;
  size_t sVar8;
  char *__s2;
  char *__s1;
  
  iVar1 = *param_2;
  if (iVar1 == *param_3) {
    if (iVar1 == 4) {
      lVar2 = *(long *)(param_2 + 2);
      lVar3 = *(long *)(param_3 + 2);
      __s1 = (char *)(lVar2 + 0x18);
      __s2 = (char *)(lVar3 + 0x18);
      uVar4 = strcoll(__s1,__s2);
      if (uVar4 == 0) {
        sVar8 = *(size_t *)(lVar3 + 0x10);
        sVar7 = *(size_t *)(lVar2 + 0x10);
        do {
          sVar6 = strlen(__s1);
          if (sVar6 == sVar8) {
            return 0;
          }
          if (sVar6 == sVar7) {
            return 1;
          }
          __s1 = __s1 + sVar6 + 1;
          sVar7 = sVar7 - (sVar6 + 1);
          __s2 = __s2 + sVar6 + 1;
          sVar8 = sVar8 - (sVar6 + 1);
          uVar4 = strcoll(__s1,__s2);
        } while (uVar4 == 0);
      }
      uVar5 = (ulong)(uVar4 >> 0x1f);
    }
    else if (iVar1 == 3) {
      uVar5 = (ulong)(*(double *)(param_2 + 2) <= *(double *)(param_3 + 2) &&
                     *(double *)(param_3 + 2) != *(double *)(param_2 + 2));
    }
    else {
      uVar5 = call_orderTM(param_1,param_2,param_3,0xb);
      if ((int)uVar5 == -1) goto LAB_0064891a;
    }
    return uVar5;
  }
LAB_0064891a:
  uVar5 = luaG_ordererror(param_1,param_2,param_3);
  return uVar5;
}



// ======== call_orderTM @ 00648990 ========

uint call_orderTM(long param_1,undefined4 *param_2,undefined4 *param_3,undefined4 param_4)

{
  int *piVar1;
  int iVar2;
  int *piVar3;
  undefined8 uVar4;
  long lVar5;
  uint uVar6;
  
  piVar3 = (int *)luaT_gettmbyobj(param_1,param_2,param_4);
  uVar6 = 0xffffffff;
  if (*piVar3 != 0) {
    uVar4 = luaT_gettmbyobj(param_1,param_3,param_4);
    iVar2 = luaO_rawequalObj(piVar3,uVar4);
    if (iVar2 != 0) {
      piVar1 = *(int **)(param_1 + 0x10);
      *piVar1 = *piVar3;
      *(undefined8 *)(piVar1 + 2) = *(undefined8 *)(piVar3 + 2);
      lVar5 = *(long *)(param_1 + 0x10);
      *(undefined4 *)(lVar5 + 0x10) = *param_2;
      *(undefined8 *)(lVar5 + 0x18) = *(undefined8 *)(param_2 + 2);
      lVar5 = *(long *)(param_1 + 0x10);
      *(undefined4 *)(lVar5 + 0x20) = *param_3;
      *(undefined8 *)(lVar5 + 0x28) = *(undefined8 *)(param_3 + 2);
      lVar5 = *(long *)(param_1 + 0x10);
      if (*(long *)(param_1 + 0x30) - lVar5 < 0x31) {
        luaD_growstack(param_1,3);
        lVar5 = *(long *)(param_1 + 0x10);
      }
      *(long *)(param_1 + 0x10) = lVar5 + 0x30;
      luaD_call(param_1,lVar5,1);
      lVar5 = *(long *)(param_1 + 0x10);
      *(long *)(param_1 + 0x10) = lVar5 + -0x10;
      uVar6 = 0;
      if (*(int *)(lVar5 + -0x10) != 0) {
        if (*(int *)(lVar5 + -0x10) == 1) {
          uVar6 = (uint)(*(int *)(lVar5 + -8) != 0);
        }
        else {
          uVar6 = 1;
        }
      }
    }
  }
  return uVar6;
}



// ======== luaV_equalval @ 00648a90 ========

bool luaV_equalval(long param_1,undefined4 *param_2,undefined4 *param_3)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  long lVar4;
  long lVar5;
  bool bVar6;
  
  bVar6 = true;
  switch(*param_2) {
  case 0:
    break;
  case 1:
    bVar6 = param_2[2] == param_3[2];
    break;
  default:
    bVar6 = *(long *)(param_2 + 2) == *(long *)(param_3 + 2);
    break;
  case 3:
    bVar6 = (bool)(-(*(double *)(param_3 + 2) == *(double *)(param_2 + 2)) & 1);
    break;
  case 5:
    if (*(long *)(param_2 + 2) == *(long *)(param_3 + 2)) {
      return true;
    }
    lVar4 = *(long *)(*(long *)(param_2 + 2) + 0x10);
    if ((*(byte *)(lVar4 + 10) & 0x10) == 0) {
      lVar5 = *(long *)(*(long *)(param_3 + 2) + 0x10);
      puVar3 = (undefined4 *)luaT_gettm(lVar4,4,*(undefined8 *)(*(long *)(param_1 + 0x20) + 200));
      if (puVar3 != (undefined4 *)0x0) {
        if (lVar4 != lVar5) {
LAB_00648b9e:
          if ((((*(byte *)(lVar5 + 10) & 0x10) != 0) ||
              (lVar4 = luaT_gettm(lVar5,4,*(undefined8 *)(*(long *)(param_1 + 0x20) + 200)),
              lVar4 == 0)) || (iVar2 = luaO_rawequalObj(puVar3,lVar4), iVar2 == 0))
          goto LAB_00648c68;
        }
LAB_00648bd9:
        puVar1 = *(undefined4 **)(param_1 + 0x10);
        *puVar1 = *puVar3;
        *(undefined8 *)(puVar1 + 2) = *(undefined8 *)(puVar3 + 2);
        lVar4 = *(long *)(param_1 + 0x10);
        *(undefined4 *)(lVar4 + 0x10) = *param_2;
        *(undefined8 *)(lVar4 + 0x18) = *(undefined8 *)(param_2 + 2);
        lVar4 = *(long *)(param_1 + 0x10);
        *(undefined4 *)(lVar4 + 0x20) = *param_3;
        *(undefined8 *)(lVar4 + 0x28) = *(undefined8 *)(param_3 + 2);
        lVar4 = *(long *)(param_1 + 0x10);
        if (*(long *)(param_1 + 0x30) - lVar4 < 0x31) {
          luaD_growstack(param_1,3);
          lVar4 = *(long *)(param_1 + 0x10);
        }
        *(long *)(param_1 + 0x10) = lVar4 + 0x30;
        luaD_call(param_1,lVar4,1);
        lVar4 = *(long *)(param_1 + 0x10);
        *(long *)(param_1 + 0x10) = lVar4 + -0x10;
        if (*(int *)(lVar4 + -0x10) != 0) {
          if (*(int *)(lVar4 + -0x10) != 1) {
            return true;
          }
          return *(int *)(lVar4 + -8) != 0;
        }
      }
    }
    goto LAB_00648c68;
  case 7:
    if (*(long *)(param_2 + 2) == *(long *)(param_3 + 2)) {
      return true;
    }
    lVar4 = *(long *)(*(long *)(param_2 + 2) + 0x10);
    if ((*(byte *)(lVar4 + 10) & 0x10) == 0) {
      lVar5 = *(long *)(*(long *)(param_3 + 2) + 0x10);
      puVar3 = (undefined4 *)luaT_gettm(lVar4,4,*(undefined8 *)(*(long *)(param_1 + 0x20) + 200));
      if (puVar3 != (undefined4 *)0x0) {
        if (lVar4 != lVar5) goto LAB_00648b9e;
        goto LAB_00648bd9;
      }
    }
LAB_00648c68:
    bVar6 = false;
  }
  return bVar6;
}



// ======== luaV_concat @ 00648c80 ========

void luaV_concat(long param_1,uint param_2,int *param_3)

{
  size_t sVar1;
  long lVar2;
  int *piVar3;
  int *extraout_RDX;
  long lVar4;
  long *plVar5;
  long lVar6;
  ulong uVar7;
  int iVar8;
  ulong uVar9;
  int iVar10;
  ulong uVar11;
  int iVar12;
  undefined4 *puVar13;
  undefined1 auVar14 [16];
  char local_58 [40];
  
  uVar9 = (ulong)param_3 & 0xffffffff;
  do {
    lVar6 = *(long *)(param_1 + 0x18);
    iVar8 = (int)uVar9;
    lVar4 = (long)iVar8 * 0x10;
    piVar3 = (int *)(lVar6 + lVar4);
    lVar2 = lVar6 + lVar4;
    puVar13 = (undefined4 *)(lVar2 + -0x10);
    iVar10 = *(int *)(lVar6 + -0x10 + lVar4);
    if (iVar10 == 4) {
LAB_00648d59:
      if (*piVar3 != 4) {
        if (*piVar3 != 3) goto LAB_00648ca0;
        sprintf(local_58,"%.14g",*(undefined8 *)(piVar3 + 2),param_3);
        *piVar3 = 4;
        sVar1 = strlen(local_58);
        auVar14 = luaS_newlstr(param_1,local_58,sVar1);
        param_3 = auVar14._8_8_;
        *(long *)(piVar3 + 2) = auVar14._0_8_;
      }
      if (*(long *)(*(long *)(piVar3 + 2) + 0x10) == 0) {
        iVar10 = 2;
        piVar3 = param_3;
      }
      else {
        uVar7 = *(long *)(*(long *)(piVar3 + 2) + 0x10) + *(long *)(*(long *)(lVar2 + -8) + 0x10);
        uVar9 = 2;
        if (2 < (int)param_2) {
          plVar5 = (long *)(lVar4 + lVar6 + -0x18);
          uVar11 = 2;
          do {
            if ((int)plVar5[-1] != 4) {
              uVar9 = uVar11;
              if ((int)plVar5[-1] != 3) break;
              sprintf(local_58,"%.14g",*plVar5);
              *(undefined4 *)(plVar5 + -1) = 4;
              sVar1 = strlen(local_58);
              lVar2 = luaS_newlstr(param_1,local_58,sVar1);
              *plVar5 = lVar2;
            }
            uVar7 = uVar7 + *(long *)(*plVar5 + 0x10);
            uVar11 = uVar11 + 1;
            plVar5 = plVar5 + -2;
            uVar9 = (ulong)param_2;
          } while (param_2 != uVar11);
        }
        iVar10 = (int)uVar9;
        if (0xfffffffffffffffd < uVar7) {
          luaG_runerror(param_1,"string size overflow");
        }
        lVar2 = luaZ_openspace(param_1,*(long *)(param_1 + 0x20) + 0x30,uVar7);
        iVar12 = iVar10 + 1;
        plVar5 = (long *)(lVar6 + ((long)iVar8 * 2 + (uVar9 & 0xffffffff) * -2) * 8 + 0x18);
        lVar6 = 0;
        do {
          sVar1 = *(size_t *)(*plVar5 + 0x10);
          memcpy((void *)(lVar6 + lVar2),(void *)(*plVar5 + 0x18),sVar1);
          lVar6 = sVar1 + lVar6;
          iVar12 = iVar12 + -1;
          plVar5 = plVar5 + 2;
        } while (1 < iVar12);
        piVar3[(uVar9 & 0xffffffff) * -4 + 4] = 4;
        auVar14 = luaS_newlstr(param_1,lVar2,lVar6);
        *(long *)(piVar3 + (uVar9 & 0xffffffff) * -4 + 4 + 2) = auVar14._0_8_;
        piVar3 = auVar14._8_8_;
      }
    }
    else {
      if (iVar10 == 3) {
        sprintf(local_58,"%.14g",*(undefined8 *)(lVar2 + -8),param_3);
        *puVar13 = 4;
        sVar1 = strlen(local_58);
        auVar14 = luaS_newlstr(param_1,local_58,sVar1);
        param_3 = auVar14._8_8_;
        *(long *)(lVar2 + -8) = auVar14._0_8_;
        goto LAB_00648d59;
      }
LAB_00648ca0:
      iVar12 = call_binTM(param_1,puVar13,piVar3,puVar13,0xd);
      iVar10 = 2;
      if (iVar12 == 0) {
        luaG_concaterror(param_1,puVar13);
        piVar3 = extraout_RDX;
      }
    }
    param_2 = param_2 - (iVar10 + -1);
    uVar9 = (ulong)(uint)(iVar8 - (iVar10 + -1));
    param_3 = piVar3;
    if ((int)param_2 < 2) {
      return;
    }
  } while( true );
}



// ======== call_binTM @ 00648f50 ========

undefined4
call_binTM(long param_1,undefined4 *param_2,undefined4 *param_3,long param_4,undefined4 param_5)

{
  int iVar1;
  undefined4 *puVar2;
  long lVar3;
  int *piVar4;
  undefined4 uVar5;
  long lVar6;
  
  param_4 = param_4 - *(long *)(param_1 + 0x38);
  piVar4 = (int *)luaT_gettmbyobj(param_1,param_2,param_5);
  iVar1 = *piVar4;
  if (iVar1 == 0) {
    piVar4 = (int *)luaT_gettmbyobj(param_1,param_3,param_5);
    iVar1 = *piVar4;
  }
  uVar5 = 0;
  if (iVar1 == 6) {
    puVar2 = *(undefined4 **)(param_1 + 0x10);
    *puVar2 = 6;
    *(undefined8 *)(puVar2 + 2) = *(undefined8 *)(piVar4 + 2);
    lVar6 = *(long *)(param_1 + 0x10);
    *(undefined4 *)(lVar6 + 0x10) = *param_2;
    *(undefined8 *)(lVar6 + 0x18) = *(undefined8 *)(param_2 + 2);
    lVar6 = *(long *)(param_1 + 0x10);
    *(undefined4 *)(lVar6 + 0x20) = *param_3;
    *(undefined8 *)(lVar6 + 0x28) = *(undefined8 *)(param_3 + 2);
    lVar6 = *(long *)(param_1 + 0x10);
    if (*(long *)(param_1 + 0x30) - lVar6 < 0x31) {
      luaD_growstack(param_1,3);
      lVar6 = *(long *)(param_1 + 0x10);
    }
    *(long *)(param_1 + 0x10) = lVar6 + 0x30;
    uVar5 = 1;
    luaD_call(param_1,lVar6,1);
    lVar6 = *(long *)(param_1 + 0x10);
    lVar3 = *(long *)(param_1 + 0x38);
    *(long *)(param_1 + 0x10) = lVar6 + -0x10;
    *(undefined4 *)(lVar3 + param_4) = *(undefined4 *)(lVar6 + -0x10);
    *(undefined8 *)(lVar3 + 8 + param_4) = *(undefined8 *)(lVar6 + -8);
  }
  return uVar5;
}



// ======== luaV_execute @ 00649040 ========

/* WARNING: Type propagation algorithm not settling */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int * luaV_execute(long param_1)

{
  byte bVar1;
  char cVar2;
  uint uVar3;
  undefined4 uVar4;
  ulong *puVar5;
  long *plVar6;
  int *piVar7;
  long lVar8;
  int *piVar9;
  undefined8 uVar10;
  int *piVar11;
  long lVar12;
  undefined8 uVar13;
  undefined4 *puVar14;
  size_t sVar15;
  ulong *puVar16;
  long lVar17;
  int *extraout_RDX;
  ulong uVar18;
  undefined8 *puVar19;
  uint uVar20;
  ulong uVar21;
  ulong uVar22;
  size_t sVar23;
  long lVar24;
  char *pcVar25;
  undefined8 in_R8;
  long lVar26;
  char *__s2;
  uint uVar27;
  int iVar28;
  long lVar29;
  int *piVar30;
  long lVar32;
  size_t sVar33;
  bool bVar34;
  double dVar35;
  ulong uStack_e0;
  undefined8 uStack_d8;
  undefined8 uStack_d0;
  int aiStack_c8 [4];
  int *piStack_b8;
  long lStack_b0;
  long lStack_a8;
  long lStack_a0;
  ulong uStack_98;
  int *piStack_90;
  ulong *local_80;
  long local_78;
  long local_70;
  long local_68;
  undefined4 local_60;
  undefined4 uStack_5c;
  undefined8 local_58;
  int *local_50;
  int *local_48;
  long local_40;
  undefined8 local_38;
  ulong uVar31;
  
  local_70 = param_1;
LAB_0064905b:
  *(ulong ***)(*(long *)(param_1 + 0x28) + 0x20) = &local_80;
  if ((*(byte *)(param_1 + 0x5c) & 1) != 0) {
    piStack_90 = (int *)0x649079;
    luaD_callhook(param_1,0,0xffffffff);
  }
LAB_00649079:
  lVar17 = *(long *)(param_1 + 0x28);
  *(undefined4 *)(lVar17 + 0x10) = 2;
  local_68 = *(long *)(*(long *)(param_1 + 0x18) + -8);
  local_40 = *(long *)(*(long *)(local_68 + 0x18) + 0x10);
  local_48 = (int *)(local_68 + 0x20);
  local_78 = local_40 + -4000;
  puVar5 = *(ulong **)(lVar17 + 0x18);
LAB_006490f5:
  do {
    puVar16 = (ulong *)((long)puVar5 + 4);
    local_80 = puVar16;
    uVar27 = (uint)*puVar5;
    bVar1 = *(byte *)(param_1 + 0x5c);
    if ((bVar1 & 0xc) != 0) {
      piVar9 = (int *)(param_1 + 100);
      *piVar9 = *piVar9 + -1;
      if (*piVar9 == 0) {
        if (bVar1 < 5) goto LAB_00649150;
        *(undefined4 *)(param_1 + 100) = *(undefined4 *)(param_1 + 0x60);
        piStack_90 = (int *)0x64913e;
        luaD_callhook(param_1,3,0xffffffff);
      }
      else {
        if ((bVar1 & 4) == 0) goto LAB_00649230;
LAB_00649150:
        plVar6 = *(long **)(param_1 + 0x28);
        puVar16 = *(ulong **)(*(long *)(*plVar6 + -8) + 0x18);
        if (puVar16[5] == 0) {
          uVar20 = 0;
          cVar2 = *(char *)(param_1 + 0x5e);
        }
        else {
          uVar20 = *(uint *)(puVar16[5] +
                            ((long)((*(long *)plVar6[4] - puVar16[3]) * 0x40000000 + -0x100000000)
                            >> 0x20) * 4);
          cVar2 = *(char *)(param_1 + 0x5e);
        }
        if (cVar2 == '\0') {
          piStack_90 = (int *)0x6491cd;
          luaG_inithooks(param_1);
          goto LAB_00649222;
        }
        uVar18 = *(ulong *)plVar6[4];
        if ((int)(uVar18 - puVar16[3] >> 2) == 1) {
          plVar6[3] = uVar18;
          uVar21 = *(ulong *)plVar6[4];
          uVar22 = uVar18;
          if (uVar21 <= uVar18) goto LAB_00649206;
LAB_006491d8:
          if (puVar16[5] == 0) {
            puVar16 = (ulong *)0x0;
            if (uVar20 != 0) goto LAB_00649206;
          }
          else {
            uVar3 = *(uint *)(puVar16[5] +
                             ((long)((uVar22 - puVar16[3]) * 0x40000000 + -0x100000000) >> 0x20) * 4
                             );
            puVar16 = (ulong *)(ulong)uVar3;
            if (uVar20 != uVar3) goto LAB_00649206;
          }
        }
        else {
          uVar22 = plVar6[3];
          uVar21 = uVar18;
          if (uVar22 < uVar18) goto LAB_006491d8;
LAB_00649206:
          piStack_90 = (int *)0x649213;
          luaD_callhook(param_1,2);
          plVar6 = *(long **)(param_1 + 0x28);
          puVar16 = (ulong *)plVar6[4];
          uVar21 = *puVar16;
        }
        plVar6[3] = uVar21;
      }
LAB_00649222:
      if ((*(byte *)(*(long *)(param_1 + 0x28) + 0x10) & 0x10) != 0) {
        *(uint **)(*(long *)(param_1 + 0x28) + 0x18) = (uint *)((long)local_80 + -4);
        uVar4 = 0x18;
        lVar17 = 0;
        goto LAB_0064a3e7;
      }
    }
LAB_00649230:
    lVar17 = local_40;
    uVar20 = uVar27 & 0x3f;
    puVar5 = local_80;
  } while (0x22 < uVar20);
  lVar24 = *(long *)(param_1 + 0x18);
  uVar3 = uVar27 >> 0x18;
  uVar18 = (ulong)uVar3;
  lVar26 = uVar18 * 0x10;
  piVar9 = (int *)(lVar24 + lVar26);
  switch((int *)(ulong)uVar20) {
  case (int *)0x0:
    uVar18 = (ulong)(uVar27 >> 0xb & 0x1ff0);
    *piVar9 = *(int *)(lVar24 + uVar18);
    *(undefined8 *)(lVar24 + 8 + lVar26) = *(undefined8 *)(lVar24 + 8 + uVar18);
    goto LAB_006490f5;
  case (int *)0x1:
    uVar18 = (ulong)(uVar27 >> 2 & 0x3ffff0);
    *piVar9 = *(int *)(local_40 + uVar18);
    *(undefined8 *)(lVar24 + 8 + lVar26) = *(undefined8 *)(local_40 + 8 + uVar18);
    goto LAB_006490f5;
  case (int *)0x2:
    *piVar9 = 1;
    *(uint *)(lVar24 + 8 + lVar26) = uVar27 >> 0xf & 0x1ff;
    if ((uVar27 & 0x7fc0) != 0) {
      puVar5 = (ulong *)((long)local_80 + 4);
    }
    goto LAB_006490f5;
  case (int *)0x3:
    piVar11 = (int *)(lVar24 + (ulong)(uVar27 >> 0xb & 0x1ff0));
    do {
      *piVar11 = 0;
      piVar11 = piVar11 + -4;
    } while (piVar9 <= piVar11);
    goto LAB_006490f5;
  case (int *)0x4:
    piVar7 = *(int **)(*(long *)(local_68 + 0x30 + (ulong)(uVar27 >> 0xc & 0xff8)) + 0x10);
    *piVar9 = *piVar7;
    break;
  case (int *)0x5:
    lVar12 = (ulong)(uVar27 >> 6 & 0x3ffff) * 0x10;
    piStack_90 = (int *)0x6497c3;
    piVar7 = (int *)luaH_getstr(*(undefined8 *)(local_68 + 0x28),
                                *(undefined8 *)(local_40 + 8 + lVar12));
    if (*piVar7 == 0) {
      piVar30 = (int *)(lVar12 + lVar17);
      piVar11 = local_48;
LAB_0064a0ce:
      piStack_90 = (int *)0x64a0d5;
      puVar14 = (undefined4 *)luaV_index(param_1,piVar11,piVar30,0);
      goto LAB_0064a0d5;
    }
    *piVar9 = *piVar7;
    break;
  case (int *)0x6:
    lVar12 = (ulong)(uVar27 >> 0xf & 0x1ff) * 0x10;
    piVar11 = (int *)(lVar24 + lVar12);
    uVar27 = uVar27 >> 6 & 0x1ff;
    lVar17 = local_78;
    if (uVar27 < 0xfa) {
      lVar17 = lVar24;
    }
    piVar30 = (int *)((ulong)uVar27 * 0x10 + lVar17);
    if (*(int *)(lVar24 + lVar12) == 5) {
      piStack_90 = (int *)0x64990f;
      piVar7 = (int *)luaH_get(*(undefined8 *)(lVar24 + 8 + lVar12),piVar30);
      iVar28 = *piVar7;
      goto joined_r0x0064944d;
    }
LAB_00649f46:
    piStack_90 = (int *)0x649f4d;
    puVar14 = (undefined4 *)luaV_getnotable(param_1,piVar11,piVar30,0);
LAB_0064a0d5:
    lVar17 = *(long *)(param_1 + 0x18);
    *(undefined4 *)(lVar17 + lVar26) = *puVar14;
    *(undefined8 *)(lVar17 + 8 + lVar26) = *(undefined8 *)(puVar14 + 2);
    puVar5 = local_80;
    goto LAB_006490f5;
  case (int *)0x7:
    piStack_90 = (int *)0x6496bd;
    luaV_settable(param_1,local_48,(ulong)(uVar27 >> 6 & 0x3ffff) * 0x10 + local_40,piVar9);
    puVar5 = local_80;
    goto LAB_006490f5;
  case (int *)0x8:
    piVar11 = *(int **)(*(long *)(local_68 + 0x30 + (ulong)(uVar27 >> 0xc & 0xff8)) + 0x10);
    *piVar11 = *piVar9;
    *(undefined8 *)(piVar11 + 2) = *(undefined8 *)(lVar24 + 8 + lVar26);
    goto LAB_006490f5;
  case (int *)0x9:
    uVar20 = uVar27 >> 0xf & 0x1ff;
    lVar17 = local_78;
    if (uVar20 < 0xfa) {
      lVar17 = lVar24;
    }
    uVar27 = uVar27 >> 6 & 0x1ff;
    if (0xf9 < uVar27) {
      lVar24 = local_78;
    }
    piStack_90 = (int *)0x64954a;
    luaV_settable(param_1,piVar9,(ulong)uVar20 * 0x10 + lVar17,(ulong)uVar27 * 0x10 + lVar24);
    puVar5 = local_80;
    goto LAB_006490f5;
  case (int *)0xa:
    *piVar9 = 5;
    piStack_90 = (int *)0x6499c0;
    uVar13 = luaH_new(param_1,(uVar27 >> 0xf & 7) << ((byte)(uVar27 >> 0x12) & 0x1f),
                      uVar27 >> 6 & 0x1ff);
    *(undefined8 *)(lVar24 + 8 + lVar26) = uVar13;
    puVar5 = local_80;
    if (*(ulong *)(*(long *)(param_1 + 0x20) + 0x40) <= *(ulong *)(*(long *)(param_1 + 0x20) + 0x48)
       ) goto LAB_00649d0e;
    goto LAB_006490f5;
  case (int *)0xb:
    uVar20 = uVar27 >> 6 & 0x1ff;
    lVar17 = local_78;
    if (uVar20 < 0xfa) {
      lVar17 = lVar24;
    }
    lVar12 = (ulong)uVar20 * 0x10;
    if (*(int *)(lVar17 + lVar12) != 4) {
      return (int *)0x0;
    }
    lVar8 = (ulong)(uVar27 >> 0xf & 0x1ff) * 0x10;
    piVar11 = (int *)(lVar24 + lVar8);
    piVar30 = (int *)(lVar17 + lVar12);
    piVar9[4] = *piVar11;
    *(undefined8 *)(piVar9 + 6) = *(undefined8 *)(lVar24 + 8 + lVar8);
    if (*piVar11 != 5) goto LAB_00649f46;
    piStack_90 = (int *)0x649449;
    local_50 = piVar30;
    piVar7 = (int *)luaH_getstr(*(undefined8 *)(lVar24 + lVar8 + 8),
                                *(undefined8 *)(lVar17 + 8 + lVar12));
    iVar28 = *piVar7;
    piVar30 = local_50;
joined_r0x0064944d:
    if (iVar28 == 0) goto LAB_0064a0ce;
    *piVar9 = iVar28;
    break;
  case (int *)0xc:
    uVar20 = uVar27 >> 0xf & 0x1ff;
    lVar17 = local_78;
    if (uVar20 < 0xfa) {
      lVar17 = lVar24;
    }
    lVar8 = (ulong)uVar20 * 0x10;
    uVar27 = uVar27 >> 6 & 0x1ff;
    lVar12 = local_78;
    if (uVar27 < 0xfa) {
      lVar12 = lVar24;
    }
    lVar29 = (ulong)uVar27 * 0x10;
    if ((*(int *)(lVar17 + lVar8) == 3) && (*(int *)(lVar12 + lVar29) == 3)) {
      *piVar9 = 3;
      *(double *)(lVar24 + 8 + lVar26) =
           *(double *)(lVar17 + 8 + lVar8) + *(double *)(lVar12 + 8 + lVar29);
    }
    else {
      in_R8 = 5;
      piStack_90 = (int *)0x649e4d;
      Arith(param_1,piVar9,lVar17 + lVar8);
      puVar5 = local_80;
    }
    goto LAB_006490f5;
  case (int *)0xd:
    uVar20 = uVar27 >> 0xf & 0x1ff;
    lVar17 = local_78;
    if (uVar20 < 0xfa) {
      lVar17 = lVar24;
    }
    lVar8 = (ulong)uVar20 * 0x10;
    uVar27 = uVar27 >> 6 & 0x1ff;
    lVar12 = local_78;
    if (uVar27 < 0xfa) {
      lVar12 = lVar24;
    }
    lVar29 = (ulong)uVar27 * 0x10;
    if ((*(int *)(lVar17 + lVar8) == 3) && (*(int *)(lVar12 + lVar29) == 3)) {
      *piVar9 = 3;
      *(double *)(lVar24 + 8 + lVar26) =
           *(double *)(lVar17 + 8 + lVar8) - *(double *)(lVar12 + 8 + lVar29);
    }
    else {
      in_R8 = 6;
      piStack_90 = (int *)0x649e83;
      Arith(param_1,piVar9,lVar17 + lVar8);
      puVar5 = local_80;
    }
    goto LAB_006490f5;
  case (int *)0xe:
    uVar20 = uVar27 >> 0xf & 0x1ff;
    lVar17 = local_78;
    if (uVar20 < 0xfa) {
      lVar17 = lVar24;
    }
    lVar8 = (ulong)uVar20 * 0x10;
    uVar27 = uVar27 >> 6 & 0x1ff;
    lVar12 = local_78;
    if (uVar27 < 0xfa) {
      lVar12 = lVar24;
    }
    lVar29 = (ulong)uVar27 * 0x10;
    if ((*(int *)(lVar17 + lVar8) == 3) && (*(int *)(lVar12 + lVar29) == 3)) {
      *piVar9 = 3;
      *(double *)(lVar24 + 8 + lVar26) =
           *(double *)(lVar17 + 8 + lVar8) * *(double *)(lVar12 + 8 + lVar29);
    }
    else {
      in_R8 = 7;
      piStack_90 = (int *)0x649e32;
      Arith(param_1,piVar9,lVar17 + lVar8);
      puVar5 = local_80;
    }
    goto LAB_006490f5;
  case (int *)0xf:
    uVar20 = uVar27 >> 0xf & 0x1ff;
    lVar17 = local_78;
    if (uVar20 < 0xfa) {
      lVar17 = lVar24;
    }
    lVar8 = (ulong)uVar20 * 0x10;
    uVar27 = uVar27 >> 6 & 0x1ff;
    lVar12 = local_78;
    if (uVar27 < 0xfa) {
      lVar12 = lVar24;
    }
    lVar29 = (ulong)uVar27 * 0x10;
    if ((*(int *)(lVar17 + lVar8) == 3) && (*(int *)(lVar12 + lVar29) == 3)) {
      *piVar9 = 3;
      *(double *)(lVar24 + 8 + lVar26) =
           *(double *)(lVar17 + 8 + lVar8) / *(double *)(lVar12 + 8 + lVar29);
    }
    else {
      in_R8 = 8;
      piStack_90 = (int *)0x649e68;
      Arith(param_1,piVar9,lVar17 + lVar8);
      puVar5 = local_80;
    }
    goto LAB_006490f5;
  case (int *)0x10:
    uVar20 = uVar27 >> 0xf & 0x1ff;
    lVar17 = local_78;
    if (uVar20 < 0xfa) {
      lVar17 = lVar24;
    }
    uVar27 = uVar27 >> 6 & 0x1ff;
    if (0xf9 < uVar27) {
      lVar24 = local_78;
    }
    in_R8 = 9;
    piStack_90 = (int *)0x649353;
    Arith(param_1,piVar9,(ulong)uVar20 * 0x10 + lVar17,(ulong)uVar27 * 0x10 + lVar24);
    puVar5 = local_80;
    goto LAB_006490f5;
  case (int *)0x11:
    lVar17 = (ulong)(uVar27 >> 0xf & 0x1ff) * 0x10;
    puVar14 = (undefined4 *)(lVar24 + lVar17);
    if (*(int *)(lVar24 + lVar17) == 3) {
LAB_00649775:
      *piVar9 = 3;
      *(ulong *)(lVar24 + 8 + lVar26) = *(ulong *)(puVar14 + 2) ^ _DAT_0066c300;
      puVar5 = local_80;
    }
    else {
      if (*(int *)(lVar24 + lVar17) == 4) {
        piStack_90 = (int *)0x649751;
        iVar28 = luaO_str2d(*(long *)(lVar24 + 8 + lVar17) + 0x18,&local_38);
        if (iVar28 != 0) {
          local_60 = 3;
          local_58 = local_38;
          puVar14 = &local_60;
          goto LAB_00649775;
        }
      }
      local_60 = 0;
      in_R8 = 10;
      piStack_90 = (int *)0x6490df;
      iVar28 = call_binTM(param_1,puVar14,&local_60,piVar9);
      puVar5 = local_80;
      if (iVar28 == 0) {
        piStack_90 = (int *)0x64a191;
        luaG_aritherror(param_1,puVar14,&local_60);
        puVar5 = local_80;
      }
    }
    goto LAB_006490f5;
  case (int *)0x12:
    lVar17 = (ulong)(uVar27 >> 0xf & 0x1ff) * 0x10;
    iVar28 = *(int *)(lVar24 + lVar17);
    if (iVar28 == 0) {
      uVar27 = 1;
    }
    else if (iVar28 == 1) {
      uVar27 = (uint)(*(int *)(lVar24 + 8 + lVar17) == 0);
    }
    else {
      uVar27 = 0;
    }
    *piVar9 = 1;
    *(uint *)(lVar24 + 8 + lVar26) = uVar27;
    goto LAB_006490f5;
  case (int *)0x13:
    uVar20 = uVar27 >> 0xf & 0x1ff;
    uVar27 = uVar27 >> 6 & 0x1ff;
    piStack_90 = (int *)0x649c1b;
    luaV_concat(param_1,(uVar27 - uVar20) + 1,uVar27);
    lVar17 = *(long *)(param_1 + 0x18);
    lVar24 = (ulong)uVar20 * 0x10;
    *(undefined4 *)(lVar17 + lVar26) = *(undefined4 *)(lVar17 + lVar24);
    *(undefined8 *)(lVar17 + 8 + lVar26) = *(undefined8 *)(lVar17 + 8 + lVar24);
    puVar5 = local_80;
    if (*(ulong *)(*(long *)(param_1 + 0x20) + 0x40) <= *(ulong *)(*(long *)(param_1 + 0x20) + 0x48)
       ) goto LAB_00649d0e;
    goto LAB_006490f5;
  case (int *)0x14:
    goto switchD_00649255_caseD_14;
  case (int *)0x15:
    uVar20 = uVar27 >> 0xf & 0x1ff;
    lVar17 = local_78;
    if (uVar20 < 0xfa) {
      lVar17 = lVar24;
    }
    lVar26 = (ulong)uVar20 * 0x10;
    uVar27 = uVar27 >> 6 & 0x1ff;
    if (0xf9 < uVar27) {
      lVar24 = local_78;
    }
    lVar12 = (ulong)uVar27 * 0x10;
    if (*(int *)(lVar17 + lVar26) == *(int *)(lVar24 + lVar12)) {
      piStack_90 = (int *)0x64982d;
      iVar28 = luaV_equalval(param_1,lVar17 + lVar26,lVar24 + lVar12);
      if (uVar3 == (iVar28 != 0)) goto LAB_0064a083;
LAB_0064a05f:
      puVar5 = (ulong *)((long)local_80 + 4);
      goto LAB_006490f5;
    }
    if (uVar3 != 0) goto LAB_0064a05f;
    goto LAB_0064a083;
  case (int *)0x16:
    uVar20 = uVar27 >> 0xf & 0x1ff;
    lVar17 = local_78;
    if (uVar20 < 0xfa) {
      lVar17 = lVar24;
    }
    uVar27 = uVar27 >> 6 & 0x1ff;
    if (0xf9 < uVar27) {
      lVar24 = local_78;
    }
    piStack_90 = (int *)0x649b52;
    uVar27 = luaV_lessthan(param_1,(ulong)uVar20 * 0x10 + lVar17,(ulong)uVar27 * 0x10 + lVar24);
    if (uVar27 == uVar3) {
      puVar5 = (ulong *)((long)local_80 + (ulong)((uint)*local_80 >> 6 & 0x3ffff) * 4 + -0x7fff8);
    }
    else {
      puVar5 = (ulong *)((long)local_80 + 4);
    }
    goto LAB_006490f5;
  case (int *)0x17:
    uVar20 = uVar27 >> 0xf & 0x1ff;
    lVar17 = local_78;
    if (uVar20 < 0xfa) {
      lVar17 = lVar24;
    }
    lVar12 = (ulong)uVar20 * 0x10;
    lVar26 = lVar17 + lVar12;
    uVar27 = uVar27 >> 6 & 0x1ff;
    if (0xf9 < uVar27) {
      lVar24 = local_78;
    }
    lVar32 = (ulong)uVar27 * 0x10;
    lVar8 = lVar24 + lVar32;
    iVar28 = *(int *)(lVar17 + lVar12);
    lVar29 = local_70;
    if (iVar28 == *(int *)(lVar24 + lVar32)) {
      if (iVar28 == 4) {
        lVar17 = *(long *)(lVar17 + 8 + lVar12);
        lVar24 = *(long *)(lVar24 + 8 + lVar32);
        pcVar25 = (char *)(lVar17 + 0x18);
        __s2 = (char *)(lVar24 + 0x18);
        piStack_90 = (int *)0x64a10f;
        uVar27 = strcoll(pcVar25,__s2);
        if (uVar27 == 0) {
          sVar33 = *(size_t *)(lVar24 + 0x10);
          sVar23 = *(size_t *)(lVar17 + 0x10);
          do {
            piStack_90 = (int *)0x64a1b8;
            sVar15 = strlen(pcVar25);
            if (sVar15 == sVar33) {
              uVar27 = (uint)(sVar33 != sVar23);
              break;
            }
            if (sVar15 == sVar23) {
              uVar27 = 0xffffffff;
              break;
            }
            pcVar25 = pcVar25 + sVar15 + 1;
            sVar23 = sVar23 - (sVar15 + 1);
            __s2 = __s2 + sVar15 + 1;
            sVar33 = sVar33 - (sVar15 + 1);
            piStack_90 = (int *)0x64a1ed;
            uVar27 = strcoll(pcVar25,__s2);
          } while (uVar27 == 0);
        }
        uVar27 = (uint)((int)uVar27 < 1);
        param_1 = local_70;
      }
      else if (iVar28 == 3) {
        uVar27 = (uint)(*(double *)(lVar17 + 8 + lVar12) <= *(double *)(lVar24 + 8 + lVar32));
        param_1 = local_70;
      }
      else {
        piStack_90 = (int *)0x64a143;
        uVar27 = call_orderTM(local_70,lVar26,lVar8,0xc);
        lVar29 = local_70;
        param_1 = local_70;
        if (uVar27 == 0xffffffff) {
          piStack_90 = (int *)0x64a210;
          iVar28 = call_orderTM(local_70,lVar8,lVar26,0xb);
          if (iVar28 == -1) goto LAB_00649f23;
          uVar27 = (uint)(iVar28 == 0);
          param_1 = local_70;
        }
      }
    }
    else {
LAB_00649f23:
      piStack_90 = (int *)0x649f2e;
      uVar27 = luaG_ordererror(lVar29,lVar26,lVar8);
      param_1 = local_70;
    }
    local_70 = param_1;
    if (uVar27 == uVar3) {
      puVar5 = (ulong *)((long)local_80 + (ulong)((uint)*local_80 >> 6 & 0x3ffff) * 4 + -0x7fff8);
    }
    else {
      puVar5 = (ulong *)((long)local_80 + 4);
    }
    goto LAB_006490f5;
  case (int *)0x18:
    lVar17 = (ulong)(uVar27 >> 0xf & 0x1ff) * 0x10;
    iVar28 = *(int *)(lVar24 + lVar17);
    if (iVar28 == 0) {
      bVar34 = true;
    }
    else if (iVar28 == 1) {
      bVar34 = *(int *)(lVar24 + 8 + lVar17) == 0;
    }
    else {
      bVar34 = false;
    }
    if ((uVar27 >> 6 & 0x1ff) == (uint)bVar34) goto LAB_0064a05f;
    *piVar9 = iVar28;
    *(undefined8 *)(lVar24 + 8 + lVar26) = *(undefined8 *)(lVar24 + 8 + lVar17);
LAB_0064a083:
    puVar5 = (ulong *)((long)local_80 + (ulong)((uint)*local_80 >> 6 & 0x3ffff) * 4 + -0x7fff8);
    goto LAB_006490f5;
  default:
    uVar3 = uVar27 >> 0xf & 0x1ff;
    if (uVar3 != 0) {
      *(int **)(param_1 + 0x10) = piVar9 + (ulong)uVar3 * 4;
    }
    piStack_90 = (int *)0x64927c;
    uVar22 = luaD_precall(param_1);
    if (uVar22 == 0) {
      if (uVar20 == 0x19) {
        *(ulong **)(*(long *)(param_1 + 0x28) + -0x18) = local_80;
        *(undefined4 *)(*(long *)(param_1 + 0x28) + -0x20) = 0xc;
      }
      else {
        lVar17 = *(long *)(*(long *)(param_1 + 0x28) + -0x30);
        lVar24 = uVar18 * 0x10;
        if (*(long *)(param_1 + 0x80) != 0) {
          piStack_90 = (int *)0x64a347;
          luaF_close(param_1);
        }
        if ((ulong)(lVar17 + lVar24) < *(ulong *)(param_1 + 0x10)) {
          uVar27 = 0;
          lVar26 = lVar17;
          do {
            *(undefined4 *)(lVar26 + -0x10) = *(undefined4 *)(lVar26 + lVar24);
            *(undefined8 *)(lVar26 + -8) = *(undefined8 *)(lVar26 + 8 + lVar24);
            uVar27 = uVar27 + 1;
            lVar12 = lVar26 + lVar24;
            lVar26 = lVar26 + 0x10;
          } while (lVar12 + 0x10U < *(ulong *)(param_1 + 0x10));
        }
        else {
          uVar27 = 0;
        }
        lVar17 = lVar17 + (ulong)uVar27 * 0x10;
        *(long *)(param_1 + 0x10) = lVar17;
        lVar24 = *(long *)(param_1 + 0x28);
        *(long *)(lVar24 + -0x28) = lVar17;
        *(undefined8 *)(lVar24 + -0x18) = *(undefined8 *)(lVar24 + 0x18);
        piVar9 = (int *)(*(long *)(param_1 + 0x28) + -8);
        *piVar9 = *piVar9 + 1;
        lVar17 = *(long *)(param_1 + 0x28);
        *(undefined4 *)(lVar17 + -0x20) = 8;
        *(long *)(param_1 + 0x28) = lVar17 + -0x30;
        *(undefined8 *)(param_1 + 0x18) = *(undefined8 *)(lVar17 + -0x30);
      }
      goto LAB_0064905b;
    }
    if (*(ulong *)(param_1 + 0x10) < uVar22) {
      *(ulong **)(*(long *)(param_1 + 0x28) + -0x18) = local_80;
      uVar4 = 8;
      lVar17 = -1;
LAB_0064a3e7:
      *(undefined4 *)(*(long *)(param_1 + 0x28) + 0x10 + lVar17 * 0x30) = uVar4;
      return (int *)0x0;
    }
    uVar27 = uVar27 >> 6 & 0x1ff;
    piStack_90 = (int *)0x6492a9;
    luaD_poscall(param_1,uVar27 - 1,uVar22);
    puVar5 = local_80;
    if (uVar27 != 0) {
      *(undefined8 *)(param_1 + 0x10) = *(undefined8 *)(*(long *)(param_1 + 0x28) + 8);
    }
    goto LAB_006490f5;
  case (int *)0x1b:
    goto switchD_00649255_caseD_1b;
  case (int *)0x1c:
    piVar11 = piVar9 + 4;
    if (*piVar9 == 3) {
      iVar28 = *piVar11;
      piVar30 = (int *)(ulong)uVar20;
      if (iVar28 == 3) goto LAB_006495e3;
LAB_00649ead:
      if (iVar28 == 4) {
        piStack_90 = (int *)0x649ecb;
        iVar28 = luaO_str2d(*(long *)(piVar9 + 6) + 0x18,&local_60);
        if (iVar28 == 0) goto LAB_00649f7f;
        *piVar11 = 3;
        *(ulong *)(piVar9 + 6) = CONCAT44(uStack_5c,local_60);
      }
      else {
LAB_00649f7f:
        piVar11 = (int *)0x0;
        piStack_90 = (int *)0x649f90;
        luaG_runerror(param_1,"`for\' limit must be a number");
      }
      iVar28 = piVar9[8];
      piVar30 = piVar9;
    }
    else {
      piStack_90 = (int *)0x649e9f;
      luaG_runerror(param_1,"`for\' initial value must be a number");
      iVar28 = *piVar11;
      piVar30 = piVar9;
      if (iVar28 != 3) goto LAB_00649ead;
LAB_006495e3:
      iVar28 = piVar9[8];
    }
    if (iVar28 != 3) {
      uVar4 = (undefined4)in_R8;
      if (iVar28 == 4) {
        piStack_90 = (int *)0x649fb9;
        iVar28 = luaO_str2d(*(long *)(piVar9 + 10) + 0x18,&local_60);
        uVar4 = (undefined4)in_R8;
        piVar30 = piVar9;
        if (iVar28 != 0) {
          piVar9[8] = 3;
          *(ulong *)(piVar9 + 10) = CONCAT44(uStack_5c,local_60);
          goto LAB_00649fd6;
        }
      }
      pcVar25 = "`for\' step must be a number";
      piStack_90 = (int *)0x64a435;
      lVar17 = param_1;
      luaG_runerror();
      piStack_90 = piVar30;
      uStack_98 = (ulong)uVar27;
      lStack_a0 = lVar26;
      lStack_a8 = param_1;
      lStack_b0 = lVar24;
      piStack_b8 = piVar11;
      piVar9 = extraout_RDX;
      if (*extraout_RDX == 3) {
LAB_0064a4a0:
        uVar27 = (uint)*puVar16;
        piVar11 = (int *)(ulong)uVar27;
        if (uVar27 != 3) {
          if ((uVar27 != 4) ||
             (piVar11 = (int *)luaO_str2d(puVar16[1] + 0x18,&uStack_d0), (int)piVar11 == 0))
          goto LAB_0064a508;
          uStack_e0._0_4_ = 3;
          uStack_d8 = uStack_d0;
          puVar16 = &uStack_e0;
        }
        switch(uVar4) {
        case 5:
          pcVar25[0] = '\x03';
          pcVar25[1] = '\0';
          pcVar25[2] = '\0';
          pcVar25[3] = '\0';
          *(double *)(pcVar25 + 8) = *(double *)(piVar9 + 2) + (double)puVar16[1];
          break;
        case 6:
          pcVar25[0] = '\x03';
          pcVar25[1] = '\0';
          pcVar25[2] = '\0';
          pcVar25[3] = '\0';
          *(double *)(pcVar25 + 8) = *(double *)(piVar9 + 2) - (double)puVar16[1];
          break;
        case 7:
          pcVar25[0] = '\x03';
          pcVar25[1] = '\0';
          pcVar25[2] = '\0';
          pcVar25[3] = '\0';
          *(double *)(pcVar25 + 8) = *(double *)(piVar9 + 2) * (double)puVar16[1];
          break;
        case 8:
          pcVar25[0] = '\x03';
          pcVar25[1] = '\0';
          pcVar25[2] = '\0';
          pcVar25[3] = '\0';
          *(double *)(pcVar25 + 8) = *(double *)(piVar9 + 2) / (double)puVar16[1];
          break;
        case 9:
          piVar11 = (int *)luaH_getstr(*(undefined8 *)(lVar17 + 0x78),
                                       *(undefined8 *)(*(long *)(lVar17 + 0x20) + 0xf0));
          iVar28 = 6;
          lVar24 = *(long *)(lVar17 + 0x38);
          if (*piVar11 != 6) {
            luaG_runerror(lVar17,"`__pow\' (`^\' operator) is not a function");
            iVar28 = *piVar11;
          }
          piVar30 = *(int **)(lVar17 + 0x10);
          *piVar30 = iVar28;
          *(undefined8 *)(piVar30 + 2) = *(undefined8 *)(piVar11 + 2);
          lVar26 = *(long *)(lVar17 + 0x10);
          *(int *)(lVar26 + 0x10) = *piVar9;
          *(undefined8 *)(lVar26 + 0x18) = *(undefined8 *)(piVar9 + 2);
          lVar26 = *(long *)(lVar17 + 0x10);
          *(uint *)(lVar26 + 0x20) = (uint)*puVar16;
          *(ulong *)(lVar26 + 0x28) = puVar16[1];
          lVar26 = *(long *)(lVar17 + 0x10);
          if (*(long *)(lVar17 + 0x30) - lVar26 < 0x31) {
            luaD_growstack(lVar17,3);
            lVar26 = *(long *)(lVar17 + 0x10);
          }
          *(long *)(lVar17 + 0x10) = lVar26 + 0x30;
          luaD_call(lVar17,lVar26,1);
          lVar26 = *(long *)(lVar17 + 0x10);
          lVar12 = *(long *)(lVar17 + 0x38);
          *(long *)(lVar17 + 0x10) = lVar26 + -0x10;
          *(undefined4 *)(pcVar25 + (lVar12 - lVar24)) = *(undefined4 *)(lVar26 + -0x10);
          piVar11 = *(int **)(lVar26 + -8);
          *(int **)(pcVar25 + ((lVar12 + 8) - lVar24)) = piVar11;
        }
      }
      else {
        if ((*extraout_RDX == 4) &&
           (iVar28 = luaO_str2d(*(long *)(extraout_RDX + 2) + 0x18,&uStack_e0), iVar28 != 0)) {
          aiStack_c8[0] = 3;
          piVar9 = aiStack_c8;
          goto LAB_0064a4a0;
        }
LAB_0064a508:
        piVar11 = (int *)call_binTM(lVar17,extraout_RDX,puVar16,pcVar25,uVar4);
        if ((int)piVar11 == 0) {
          piVar11 = (int *)luaG_aritherror(lVar17,extraout_RDX,puVar16);
        }
      }
      return piVar11;
    }
LAB_00649fd6:
    dVar35 = *(double *)(lVar24 + 8 + lVar26) + *(double *)(piVar9 + 10);
    puVar5 = local_80;
    if (*(double *)(piVar9 + 10) <= 0.0) {
      if (dVar35 < *(double *)(piVar11 + 2)) goto LAB_006490f5;
    }
    else if (*(double *)(piVar11 + 2) < dVar35) goto LAB_006490f5;
    *(double *)(lVar24 + lVar26 + 8) = dVar35;
    puVar5 = (ulong *)((long)local_80 + (ulong)(uVar27 >> 6 & 0x3ffff) * 4 + -0x7fffc);
    goto LAB_006490f5;
  case (int *)0x1d:
    uVar27 = uVar27 >> 6 & 0x1ff;
    uVar31 = (ulong)uVar27;
    uVar22 = uVar31 + 1;
    piVar9[uVar22 * 4 + 8] = *piVar9;
    *(undefined8 *)(piVar9 + uVar22 * 4 + 10) = *(undefined8 *)(lVar24 + 8 + lVar26);
    piVar9[uVar22 * 4 + 0xc] = piVar9[4];
    *(undefined8 *)(piVar9 + uVar22 * 4 + 0xe) = *(undefined8 *)(piVar9 + 6);
    piVar9[uVar22 * 4 + 0x10] = piVar9[8];
    *(undefined8 *)(piVar9 + uVar22 * 4 + 0x12) = *(undefined8 *)(piVar9 + 10);
    *(int **)(param_1 + 0x10) = piVar9 + uVar22 * 4 + 0x14;
    piStack_90 = (int *)0x649a4a;
    luaD_call(param_1,piVar9 + uVar22 * 4 + 8,uVar27 + 1);
    lVar17 = *(long *)(param_1 + 0x18);
    *(undefined8 *)(param_1 + 0x10) = *(undefined8 *)(*(long *)(param_1 + 0x28) + 8);
    piVar9 = (int *)(lVar17 + lVar26 + 0x20);
    uVar21 = uVar22;
    if ((uVar22 & 1) != 0) {
      lVar24 = lVar26 + lVar17 + uVar22 * 0x10;
      lVar12 = uVar31 * 0x10;
      piVar9[uVar31 * 4] = *(int *)(lVar24 + 0x20 + lVar12);
      *(undefined8 *)(lVar17 + lVar26 + 0x28 + lVar12) = *(undefined8 *)(lVar24 + 0x28 + lVar12);
      uVar21 = uVar31;
    }
    if (uVar27 != 0) {
      puVar19 = (undefined8 *)(lVar17 + (uVar18 * 2 + uVar21 * 2) * 8 + 0x18);
      uVar21 = uVar21 + 1;
      do {
        *(undefined4 *)(puVar19 + -1) = *(undefined4 *)(puVar19 + uVar22 * 2 + -1);
        *puVar19 = puVar19[uVar22 * 2];
        *(undefined4 *)(puVar19 + -3) = *(undefined4 *)(puVar19 + uVar22 * 2 + -3);
        puVar19[-2] = puVar19[uVar22 * 2 + -2];
        puVar19 = puVar19 + -4;
        uVar21 = uVar21 - 2;
      } while (1 < uVar21);
    }
    if (*piVar9 == 0) {
      lVar17 = 1;
    }
    else {
      lVar17 = (long)(int)(((uint)*local_80 >> 6 & 0x3ffff) - 0x1fffe);
    }
    puVar5 = (ulong *)((long)local_80 + lVar17 * 4);
    param_1 = local_70;
    goto LAB_006490f5;
  case (int *)0x1e:
    if (*piVar9 == 5) {
      piVar9[4] = 5;
      *(undefined8 *)(piVar9 + 6) = *(undefined8 *)(lVar24 + 8 + lVar26);
      uVar13 = *(undefined8 *)(param_1 + 0x78);
      piStack_90 = (int *)0x649bb0;
      uVar10 = luaS_newlstr(param_1,&DAT_00669b27,4);
      piStack_90 = (int *)0x649bbb;
      piVar11 = (int *)luaH_getstr(uVar13,uVar10);
      *piVar9 = *piVar11;
      *(undefined8 *)(lVar24 + 8 + lVar26) = *(undefined8 *)(piVar11 + 2);
    }
switchD_00649255_caseD_14:
    puVar5 = (ulong *)((long)local_80 + (ulong)(uVar27 >> 6 & 0x3ffff) * 4 + -0x7fffc);
    goto LAB_006490f5;
  case (int *)0x1f:
  case (int *)0x20:
    if (*piVar9 != 5) {
      return (int *)0x0;
    }
    uVar13 = *(undefined8 *)(lVar24 + 8 + lVar26);
    if (uVar20 == 0x1f) {
      uVar20 = (uVar27 >> 6 & 0x1f) + 1;
    }
    else {
      uVar20 = (int)((ulong)(*(long *)(param_1 + 0x10) - (long)piVar9) >> 4) - 1;
      *(undefined8 *)(param_1 + 0x10) = *(undefined8 *)(*(long *)(param_1 + 0x28) + 8);
      if ((int)uVar20 < 1) goto LAB_006490f5;
    }
    iVar28 = (uVar27 >> 6 & 0x3ffe0) + uVar20;
    uVar22 = (ulong)uVar20 + 1;
    puVar19 = (undefined8 *)(lVar24 + ((ulong)uVar20 * 2 + uVar18 * 2) * 8 + 8);
    do {
      piStack_90 = (int *)0x649dfe;
      puVar14 = (undefined4 *)luaH_setnum(param_1,uVar13,iVar28);
      *puVar14 = *(undefined4 *)(puVar19 + -1);
      *(undefined8 *)(puVar14 + 2) = *puVar19;
      iVar28 = iVar28 + -1;
      uVar22 = uVar22 - 1;
      puVar19 = puVar19 + -2;
      puVar5 = local_80;
    } while (1 < uVar22);
    goto LAB_006490f5;
  case (int *)0x21:
    piStack_90 = (int *)0x6495c1;
    luaF_close(param_1);
    puVar5 = local_80;
    goto LAB_006490f5;
  case (int *)0x22:
    lVar17 = *(long *)(*(long *)(*(long *)(local_68 + 0x18) + 0x20) +
                      (ulong)(uVar27 >> 6 & 0x3ffff) * 8);
    bVar1 = *(byte *)(lVar17 + 0x70);
    piStack_90 = (int *)0x649c81;
    local_50 = piVar9;
    lVar12 = luaF_newLclosure(param_1,(ulong)bVar1,local_48);
    *(long *)(lVar12 + 0x18) = lVar17;
    if (bVar1 != 0) {
      uVar18 = 0;
      do {
        uVar22 = (ulong)((uint)*local_80 >> 0xf & 0x1ff);
        if (((uint)*local_80 & 0x3f) == 4) {
          uVar13 = *(undefined8 *)(local_68 + 0x30 + uVar22 * 8);
        }
        else {
          piStack_90 = (int *)0x649caf;
          uVar13 = luaF_findupval(param_1,uVar22 * 0x10 + lVar24);
        }
        *(undefined8 *)(lVar12 + 0x30 + uVar18 * 8) = uVar13;
        uVar18 = uVar18 + 1;
        local_80 = (ulong *)((long)local_80 + 4);
      } while (bVar1 != uVar18);
    }
    *local_50 = 6;
    *(long *)(lVar24 + 8 + lVar26) = lVar12;
    puVar5 = local_80;
    if (*(ulong *)(*(long *)(param_1 + 0x20) + 0x40) <= *(ulong *)(*(long *)(param_1 + 0x20) + 0x48)
       ) {
LAB_00649d0e:
      piStack_90 = (int *)0x649d16;
      luaC_collectgarbage(param_1);
      puVar5 = local_80;
    }
    goto LAB_006490f5;
  }
  *(undefined8 *)(lVar24 + 8 + lVar26) = *(undefined8 *)(piVar7 + 2);
  puVar5 = local_80;
  goto LAB_006490f5;
switchD_00649255_caseD_1b:
  uVar27 = uVar27 >> 0xf & 0x1ff;
  if (uVar27 != 0) {
    *(int **)(param_1 + 0x10) = piVar9 + (ulong)uVar27 * 4 + -4;
  }
  lVar17 = *(long *)(param_1 + 0x28);
  lVar26 = lVar17;
  if (*(long *)(param_1 + 0x80) != 0) {
    piStack_90 = (int *)0x64a2a0;
    luaF_close(param_1,lVar24);
    lVar26 = *(long *)(param_1 + 0x28);
  }
  *(undefined4 *)(lVar26 + 0x10) = 8;
  *(ulong **)(lVar26 + 0x18) = local_80;
  if ((*(byte *)(lVar17 + -0x20) & 4) == 0) {
    return piVar9;
  }
  uVar27 = *(uint *)(*(long *)(lVar17 + -0x18) + -4) >> 6 & 0x1ff;
  piStack_90 = (int *)0x64a2df;
  luaD_poscall(param_1,uVar27 - 1,piVar9);
  if (uVar27 != 0) {
    *(undefined8 *)(param_1 + 0x10) = *(undefined8 *)(*(long *)(param_1 + 0x28) + 8);
  }
  goto LAB_00649079;
}



// ======== Arith @ 0064a440 ========

void Arith(long param_1,undefined4 *param_2,int *param_3,int *param_4,undefined4 param_5)

{
  long lVar1;
  int *piVar2;
  long lVar3;
  int iVar4;
  int *piVar5;
  int *piVar6;
  long lVar7;
  int local_58 [2];
  undefined8 local_50;
  undefined8 local_48;
  int local_40 [4];
  
  piVar6 = param_3;
  if (*param_3 == 3) {
LAB_0064a4a0:
    if (*param_4 != 3) {
      if ((*param_4 != 4) ||
         (iVar4 = luaO_str2d(*(long *)(param_4 + 2) + 0x18,&local_48), iVar4 == 0))
      goto LAB_0064a508;
      local_58[0] = 3;
      local_50 = local_48;
      param_4 = local_58;
    }
    switch(param_5) {
    case 5:
      *param_2 = 3;
      *(double *)(param_2 + 2) = *(double *)(piVar6 + 2) + *(double *)(param_4 + 2);
      break;
    case 6:
      *param_2 = 3;
      *(double *)(param_2 + 2) = *(double *)(piVar6 + 2) - *(double *)(param_4 + 2);
      break;
    case 7:
      *param_2 = 3;
      *(double *)(param_2 + 2) = *(double *)(piVar6 + 2) * *(double *)(param_4 + 2);
      break;
    case 8:
      *param_2 = 3;
      *(double *)(param_2 + 2) = *(double *)(piVar6 + 2) / *(double *)(param_4 + 2);
      break;
    case 9:
      piVar5 = (int *)luaH_getstr(*(undefined8 *)(param_1 + 0x78),
                                  *(undefined8 *)(*(long *)(param_1 + 0x20) + 0xf0));
      iVar4 = 6;
      lVar1 = *(long *)(param_1 + 0x38);
      if (*piVar5 != 6) {
        luaG_runerror(param_1,"`__pow\' (`^\' operator) is not a function");
        iVar4 = *piVar5;
      }
      piVar2 = *(int **)(param_1 + 0x10);
      *piVar2 = iVar4;
      *(undefined8 *)(piVar2 + 2) = *(undefined8 *)(piVar5 + 2);
      lVar7 = *(long *)(param_1 + 0x10);
      *(int *)(lVar7 + 0x10) = *piVar6;
      *(undefined8 *)(lVar7 + 0x18) = *(undefined8 *)(piVar6 + 2);
      lVar7 = *(long *)(param_1 + 0x10);
      *(int *)(lVar7 + 0x20) = *param_4;
      *(undefined8 *)(lVar7 + 0x28) = *(undefined8 *)(param_4 + 2);
      lVar7 = *(long *)(param_1 + 0x10);
      if (*(long *)(param_1 + 0x30) - lVar7 < 0x31) {
        luaD_growstack(param_1,3);
        lVar7 = *(long *)(param_1 + 0x10);
      }
      *(long *)(param_1 + 0x10) = lVar7 + 0x30;
      luaD_call(param_1,lVar7,1);
      lVar7 = *(long *)(param_1 + 0x10);
      lVar3 = *(long *)(param_1 + 0x38);
      *(long *)(param_1 + 0x10) = lVar7 + -0x10;
      *(undefined4 *)((long)param_2 + (lVar3 - lVar1)) = *(undefined4 *)(lVar7 + -0x10);
      *(undefined8 *)((long)param_2 + ((lVar3 + 8) - lVar1)) = *(undefined8 *)(lVar7 + -8);
    }
  }
  else {
    if ((*param_3 == 4) && (iVar4 = luaO_str2d(*(long *)(param_3 + 2) + 0x18,local_58), iVar4 != 0))
    {
      local_40[0] = 3;
      piVar6 = local_40;
      goto LAB_0064a4a0;
    }
LAB_0064a508:
    iVar4 = call_binTM(param_1,param_3,param_4,param_2,param_5);
    if (iVar4 == 0) {
      luaG_aritherror(param_1,param_3,param_4);
    }
  }
  return;
}



// ======== luaZ_fill @ 0064a660 ========

ulong luaZ_fill(long *param_1)

{
  byte *pbVar1;
  ulong uVar2;
  long local_10;
  
  pbVar1 = (byte *)(*(code *)param_1[2])(0,param_1[3],&local_10);
  uVar2 = 0xffffffff;
  if ((pbVar1 != (byte *)0x0) && (local_10 != 0)) {
    *param_1 = local_10 + -1;
    param_1[1] = (long)(pbVar1 + 1);
    uVar2 = (ulong)*pbVar1;
  }
  return uVar2;
}



// ======== luaZ_lookahead @ 0064a6b0 ========

ulong luaZ_lookahead(long *param_1)

{
  byte *pbVar1;
  long local_10;
  
  if (*param_1 != 0) {
    return (ulong)*(byte *)param_1[1];
  }
  pbVar1 = (byte *)(*(code *)param_1[2])(0,param_1[3],&local_10);
  if ((pbVar1 != (byte *)0x0) && (local_10 != 0)) {
    *param_1 = local_10;
    param_1[1] = (long)pbVar1;
    return (ulong)*pbVar1;
  }
  return 0xffffffff;
}



// ======== luaZ_init @ 0064a710 ========

void luaZ_init(undefined8 *param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  param_1[2] = param_2;
  param_1[3] = param_3;
  param_1[4] = param_4;
  *param_1 = 0;
  param_1[1] = 0;
  return;
}



// ======== luaZ_read @ 0064a730 ========

ulong luaZ_read(ulong *param_1,void *param_2,ulong param_3)

{
  ulong uVar1;
  void *__src;
  ulong __n;
  ulong local_30;
  
  if (param_3 != 0) {
    uVar1 = *param_1;
    do {
      if (uVar1 == 0) {
        __src = (void *)(*(code *)param_1[2])(0,param_1[3],&local_30);
        if (__src == (void *)0x0) {
          return param_3;
        }
        if (local_30 == 0) {
          return param_3;
        }
        *param_1 = local_30;
        param_1[1] = (ulong)__src;
        uVar1 = local_30;
      }
      else {
        __src = (void *)param_1[1];
      }
      __n = uVar1;
      if (param_3 <= uVar1) {
        __n = param_3;
      }
      memcpy(param_2,__src,__n);
      uVar1 = *param_1 - __n;
      *param_1 = uVar1;
      param_1[1] = param_1[1] + __n;
      param_2 = (void *)((long)param_2 + __n);
      param_3 = param_3 - __n;
    } while (param_3 != 0);
  }
  return 0;
}



// ======== luaZ_openspace @ 0064a7d0 ========

undefined8 luaZ_openspace(undefined8 param_1,undefined8 *param_2,ulong param_3)

{
  undefined8 uVar1;
  ulong uVar2;
  
  if ((ulong)param_2[1] < param_3) {
    uVar2 = 0x20;
    if (0x20 < param_3) {
      uVar2 = param_3;
    }
    uVar1 = luaM_realloc(param_1,*param_2,param_2[1],uVar2);
    *param_2 = uVar1;
    param_2[1] = uVar2;
    return uVar1;
  }
  return *param_2;
}



// ======== luaK_nil @ 0064a820 ========

void luaK_nil(long *param_1,int param_2,int param_3)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  
  iVar1 = (int)param_1[6];
  if (*(int *)((long)param_1 + 0x34) < iVar1) {
    uVar2 = *(uint *)(*(long *)(*param_1 + 0x18) + -4 + (long)iVar1 * 4);
    if (((uVar2 & 0x3f) == 3) && ((int)(uVar2 >> 0x18) <= param_2)) {
      uVar4 = uVar2 >> 0xf & 0x1ff;
      if (param_2 <= (int)(uVar4 + 1)) {
        uVar3 = (param_3 + param_2) - 1;
        if ((int)uVar4 < (int)uVar3) {
          *(uint *)(*(long *)(*param_1 + 0x18) + -4 + (long)iVar1 * 4) =
               (uVar3 & 0x1ff) << 0xf | uVar2 & 0xff007fff;
        }
        return;
      }
    }
  }
  luaK_code(param_1,(param_3 + param_2) * 0x8000 - 0x8000U | param_2 << 0x18 | 3,
            *(undefined4 *)(param_1[3] + 8));
  return;
}



// ======== luaK_codeABC @ 0064a8b0 ========

void luaK_codeABC(long param_1,uint param_2,int param_3,int param_4,int param_5)

{
  luaK_code(param_1,param_2 | param_3 << 0x18 | param_5 << 6 | param_4 << 0xf,
            *(undefined4 *)(*(long *)(param_1 + 0x18) + 8));
  return;
}



// ======== luaK_jump @ 0064a8d0 ========

uint luaK_jump(long *param_1)

{
  long lVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  long lVar7;
  uint uVar8;
  
  uVar8 = *(uint *)(param_1 + 7);
  *(undefined4 *)(param_1 + 7) = 0xffffffff;
  uVar2 = luaK_code(param_1,0x7fff94,*(undefined4 *)(param_1[3] + 8));
  uVar6 = uVar2;
  if ((uVar8 != 0xffffffff) && (uVar6 = uVar8, uVar2 != 0xffffffff)) {
    lVar1 = *(long *)(*param_1 + 0x18);
    uVar6 = uVar2;
    do {
      uVar4 = uVar6;
      lVar7 = (long)(int)uVar4;
      uVar3 = *(uint *)(lVar1 + lVar7 * 4);
      uVar5 = uVar3 >> 6 & 0x3ffff;
      uVar6 = (uVar4 - 0x1fffe) + uVar5;
      if (uVar5 == 0x1fffe) {
        uVar6 = 0xffffffff;
      }
    } while (uVar6 != 0xffffffff);
    uVar8 = uVar8 + ~uVar4;
    uVar6 = -uVar8;
    if ((int)-uVar8 < 0) {
      uVar6 = uVar8;
    }
    if (0x1ffff < uVar6) {
      luaX_syntaxerror(param_1[3],"control structure too long");
      uVar3 = *(uint *)(lVar1 + lVar7 * 4);
    }
    *(uint *)(lVar1 + lVar7 * 4) = uVar8 * 0x40 + 0x7fffc0 & 0xffffc0 | uVar3 & 0xff00003f;
    uVar6 = uVar2;
  }
  return uVar6;
}



// ======== luaK_codeABx @ 0064a9b0 ========

void luaK_codeABx(long param_1,uint param_2,int param_3,int param_4)

{
  luaK_code(param_1,param_2 | param_3 << 0x18 | param_4 << 6,
            *(undefined4 *)(*(long *)(param_1 + 0x18) + 8));
  return;
}



// ======== luaK_concat @ 0064a9d0 ========

void luaK_concat(long *param_1,uint *param_2,uint param_3)

{
  long lVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  long lVar6;
  
  if (param_3 != 0xffffffff) {
    if (*param_2 == 0xffffffff) {
      *param_2 = param_3;
      return;
    }
    lVar1 = *(long *)(*param_1 + 0x18);
    uVar5 = *param_2;
    do {
      uVar3 = uVar5;
      lVar6 = (long)(int)uVar3;
      uVar2 = *(uint *)(lVar1 + lVar6 * 4);
      uVar4 = uVar2 >> 6 & 0x3ffff;
      uVar5 = (uVar3 - 0x1fffe) + uVar4;
      if (uVar4 == 0x1fffe) {
        uVar5 = 0xffffffff;
      }
    } while (uVar5 != 0xffffffff);
    param_3 = ~uVar3 + param_3;
    uVar5 = -param_3;
    if ((int)-param_3 < 0) {
      uVar5 = param_3;
    }
    if (0x1ffff < uVar5) {
      luaX_syntaxerror(param_1[3],"control structure too long");
      uVar2 = *(uint *)(lVar1 + lVar6 * 4);
    }
    *(uint *)(lVar1 + lVar6 * 4) = param_3 * 0x40 + 0x7fffc0 & 0xffffc0 | uVar2 & 0xff00003f;
  }
  return;
}



// ======== luaK_getlabel @ 0064aa70 ========

void luaK_getlabel(long param_1)

{
  *(undefined4 *)(param_1 + 0x34) = *(undefined4 *)(param_1 + 0x30);
  return;
}



// ======== luaK_patchlist @ 0064aa80 ========

void luaK_patchlist(long *param_1,int param_2,int param_3)

{
  long lVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  long lVar5;
  uint uVar6;
  
  if ((int)param_1[6] == param_3) {
    *(int *)((long)param_1 + 0x34) = param_3;
    if (param_2 != -1) {
      if (*(uint *)(param_1 + 7) == 0xffffffff) {
        *(int *)(param_1 + 7) = param_2;
      }
      else {
        lVar1 = *(long *)(*param_1 + 0x18);
        uVar4 = *(uint *)(param_1 + 7);
        do {
          uVar6 = uVar4;
          lVar5 = (long)(int)uVar6;
          uVar2 = *(uint *)(lVar1 + lVar5 * 4);
          uVar3 = uVar2 >> 6 & 0x3ffff;
          uVar4 = (uVar6 - 0x1fffe) + uVar3;
          if (uVar3 == 0x1fffe) {
            uVar4 = 0xffffffff;
          }
        } while (uVar4 != 0xffffffff);
        uVar6 = ~uVar6 + param_2;
        uVar4 = -uVar6;
        if ((int)-uVar6 < 0) {
          uVar4 = uVar6;
        }
        if (0x1ffff < uVar4) {
          luaX_syntaxerror(param_1[3],"control structure too long");
          uVar2 = *(uint *)(lVar1 + lVar5 * 4);
        }
        *(uint *)(lVar1 + lVar5 * 4) = uVar6 * 0x40 + 0x7fffc0 & 0xffffc0 | uVar2 & 0xff00003f;
      }
    }
  }
  else {
    luaK_patchlistaux();
  }
  return;
}



// ======== luaK_patchtohere @ 0064ab50 ========

void luaK_patchtohere(long *param_1,int param_2)

{
  long lVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  long lVar5;
  uint uVar6;
  
  *(int *)((long)param_1 + 0x34) = (int)param_1[6];
  if (param_2 != -1) {
    if (*(uint *)(param_1 + 7) == 0xffffffff) {
      *(int *)(param_1 + 7) = param_2;
      return;
    }
    lVar1 = *(long *)(*param_1 + 0x18);
    uVar4 = *(uint *)(param_1 + 7);
    do {
      uVar6 = uVar4;
      lVar5 = (long)(int)uVar6;
      uVar2 = *(uint *)(lVar1 + lVar5 * 4);
      uVar3 = uVar2 >> 6 & 0x3ffff;
      uVar4 = (uVar6 - 0x1fffe) + uVar3;
      if (uVar3 == 0x1fffe) {
        uVar4 = 0xffffffff;
      }
    } while (uVar4 != 0xffffffff);
    uVar6 = ~uVar6 + param_2;
    uVar4 = -uVar6;
    if ((int)-uVar6 < 0) {
      uVar4 = uVar6;
    }
    if (0x1ffff < uVar4) {
      luaX_syntaxerror(param_1[3],"control structure too long");
      uVar2 = *(uint *)(lVar1 + lVar5 * 4);
    }
    *(uint *)(lVar1 + lVar5 * 4) = uVar6 * 0x40 + 0x7fffc0 & 0xffffc0 | uVar2 & 0xff00003f;
  }
  return;
}



// ======== luaK_patchlistaux @ 0064abf0 ========

void luaK_patchlistaux(long *param_1,uint param_2,int param_3,uint param_4,int param_5,uint param_6,
                      int param_7)

{
  uint *puVar1;
  uint uVar2;
  uint uVar3;
  uint *puVar4;
  uint uVar5;
  uint uVar6;
  
  do {
    if (param_2 == 0xffffffff) {
      return;
    }
    puVar1 = (uint *)(*(long *)(*param_1 + 0x18) + (long)(int)param_2 * 4);
    uVar2 = *(uint *)(*(long *)(*param_1 + 0x18) + (long)(int)param_2 * 4);
    uVar3 = uVar2 >> 6 & 0x3ffff;
    uVar6 = (param_2 - 0x1fffe) + uVar3;
    if (uVar3 == 0x1fffe) {
      uVar6 = 0xffffffff;
    }
    if (((int)param_2 < 1) || (uVar3 = puVar1[-1], -1 < (char)luaP_opmodes[uVar3 & 0x3f])) {
      puVar4 = puVar1;
      uVar3 = uVar2;
      if ((uVar2 & 0x3f) != 0x18) goto LAB_0064acfe;
LAB_0064acad:
      if ((uVar3 & 0x7fc0) == 0) {
        uVar2 = uVar3 >> 0xf;
        if (param_6 != 0xff) {
          uVar2 = param_6;
        }
        *puVar4 = uVar2 << 0x18 | uVar3 & 0xffffff;
        uVar5 = ~param_2 + param_5;
        uVar2 = -uVar5;
        if ((int)-uVar5 < 0) {
          uVar2 = uVar5;
        }
      }
      else {
        uVar2 = uVar3 >> 0xf;
        if (param_4 != 0xff) {
          uVar2 = param_4;
        }
        *puVar4 = uVar2 << 0x18 | uVar3 & 0xffffff;
        uVar5 = ~param_2 + param_3;
        uVar2 = -uVar5;
        if ((int)-uVar5 < 0) {
          uVar2 = uVar5;
        }
      }
      if (0x1ffff < uVar2) {
LAB_0064ac20:
        luaX_syntaxerror(param_1[3],"control structure too long");
      }
      uVar2 = *puVar1;
    }
    else {
      puVar4 = puVar1 + -1;
      if ((uVar3 & 0x3f) == 0x18) goto LAB_0064acad;
LAB_0064acfe:
      uVar5 = ~param_2 + param_7;
      uVar3 = -uVar5;
      if ((int)-uVar5 < 0) {
        uVar3 = uVar5;
      }
      if (0x1ffff < uVar3) goto LAB_0064ac20;
    }
    *puVar1 = uVar5 * 0x40 + 0x7fffc0 & 0xffffc0 | uVar2 & 0xff00003f;
    param_2 = uVar6;
  } while( true );
}



// ======== luaK_checkstack @ 0064ad60 ========

void luaK_checkstack(long *param_1,int param_2)

{
  long lVar1;
  
  param_2 = param_2 + *(int *)((long)param_1 + 0x3c);
  lVar1 = *param_1;
  if ((int)(uint)*(byte *)(lVar1 + 0x73) < param_2) {
    if (0xf9 < param_2) {
      luaX_syntaxerror(param_1[3],"function or expression too complex");
      lVar1 = *param_1;
    }
    *(char *)(lVar1 + 0x73) = (char)param_2;
  }
  return;
}



// ======== luaK_reserveregs @ 0064ada0 ========

void luaK_reserveregs(long *param_1,int param_2)

{
  long lVar1;
  int iVar2;
  int iVar3;
  
  iVar2 = *(int *)((long)param_1 + 0x3c) + param_2;
  lVar1 = *param_1;
  iVar3 = iVar2;
  if ((int)(uint)*(byte *)(lVar1 + 0x73) < iVar2) {
    if (0xf9 < iVar2) {
      luaX_syntaxerror(param_1[3],"function or expression too complex");
      lVar1 = *param_1;
      iVar3 = param_2 + *(int *)((long)param_1 + 0x3c);
    }
    *(char *)(lVar1 + 0x73) = (char)iVar2;
  }
  *(int *)((long)param_1 + 0x3c) = iVar3;
  return;
}



// ======== luaK_stringK @ 0064adf0 ========

void luaK_stringK(undefined8 param_1,undefined8 param_2)

{
  undefined4 local_10 [2];
  undefined8 local_8;
  
  local_10[0] = 4;
  local_8 = param_2;
  addk(param_1,local_10);
  return;
}



// ======== addk @ 0064ae20 ========

int addk(long *param_1,undefined8 param_2,undefined4 *param_3)

{
  long lVar1;
  int iVar2;
  int *piVar3;
  long lVar4;
  undefined4 *puVar5;
  
  piVar3 = (int *)luaH_get(param_1[1]);
  if (*piVar3 == 3) {
    iVar2 = (int)*(double *)(piVar3 + 2);
  }
  else {
    lVar1 = *param_1;
    iVar2 = (int)param_1[8];
    if (iVar2 < *(int *)(lVar1 + 0x4c)) {
      lVar4 = *(long *)(lVar1 + 0x10);
    }
    else {
      lVar4 = luaM_growaux(param_1[4],*(undefined8 *)(lVar1 + 0x10),lVar1 + 0x4c,0x10,0x3ffff,
                           "constant table overflow");
      *(long *)(lVar1 + 0x10) = lVar4;
      iVar2 = (int)param_1[8];
    }
    *(undefined4 *)(lVar4 + (long)iVar2 * 0x10) = *param_3;
    *(undefined8 *)(lVar4 + 8 + (long)iVar2 * 0x10) = *(undefined8 *)(param_3 + 2);
    puVar5 = (undefined4 *)luaH_set(param_1[4],param_1[1],param_2);
    *puVar5 = 3;
    *(double *)(puVar5 + 2) = (double)(int)param_1[8];
    iVar2 = (int)param_1[8];
    *(int *)(param_1 + 8) = iVar2 + 1;
  }
  return iVar2;
}



// ======== luaK_numberK @ 0064aee0 ========

void luaK_numberK(undefined8 param_1,undefined8 param_2)

{
  undefined4 local_10 [2];
  undefined8 local_8;
  
  local_10[0] = 3;
  local_8 = param_1;
  addk(param_2,local_10);
  return;
}



// ======== luaK_setcallreturns @ 0064af10 ========

void luaK_setcallreturns(long *param_1,int *param_2,int param_3)

{
  long lVar1;
  
  if (*param_2 == 0xc) {
    lVar1 = *(long *)(*param_1 + 0x18);
    *(uint *)(lVar1 + (long)param_2[1] * 4) =
         param_3 * 0x40 + 0x40U & 0x7fc0 | *(uint *)(lVar1 + (long)param_2[1] * 4) & 0xffff803f;
    if (param_3 == 1) {
      *param_2 = 0xb;
      param_2[1] = (uint)*(byte *)(lVar1 + 3 + (long)param_2[1] * 4);
    }
  }
  return;
}



// ======== luaK_dischargevars @ 0064af60 ========

void luaK_dischargevars(long *param_1,undefined4 *param_2)

{
  int iVar1;
  int iVar2;
  long lVar3;
  undefined4 uVar4;
  uint uVar5;
  
  switch(*param_2) {
  case 5:
    *param_2 = 0xb;
    return;
  case 6:
    uVar5 = param_2[1] << 0xf | 4;
    break;
  case 7:
    uVar5 = param_2[1] << 6 | 5;
    break;
  case 8:
    iVar1 = param_2[2];
    if ((iVar1 < 0xfa) && (*(int *)((long)param_1 + 0x4c) <= iVar1)) {
      *(int *)((long)param_1 + 0x3c) = *(int *)((long)param_1 + 0x3c) + -1;
    }
    iVar2 = param_2[1];
    if ((iVar2 < 0xfa) && (*(int *)((long)param_1 + 0x4c) <= iVar2)) {
      *(int *)((long)param_1 + 0x3c) = *(int *)((long)param_1 + 0x3c) + -1;
    }
    uVar5 = iVar1 << 6 | iVar2 << 0xf | 6;
    break;
  default:
    return;
  case 0xc:
    lVar3 = *(long *)(*param_1 + 0x18);
    *(uint *)(lVar3 + (long)(int)param_2[1] * 4) =
         *(uint *)(lVar3 + (long)(int)param_2[1] * 4) & 0xffff803f | 0x80;
    *param_2 = 0xb;
    param_2[1] = (uint)*(byte *)(lVar3 + 3 + (long)(int)param_2[1] * 4);
    return;
  }
  uVar4 = luaK_code(param_1,uVar5,*(undefined4 *)(param_1[3] + 8));
  param_2[1] = uVar4;
  *param_2 = 10;
  return;
}



// ======== luaK_exp2nextreg @ 0064b020 ========

void luaK_exp2nextreg(long *param_1,int *param_2)

{
  int iVar1;
  long lVar2;
  int iVar3;
  int iVar4;
  
  luaK_dischargevars();
  if (((*param_2 == 0xb) && (param_2[1] < 0xfa)) && (*(int *)((long)param_1 + 0x4c) <= param_2[1]))
  {
    *(int *)((long)param_1 + 0x3c) = *(int *)((long)param_1 + 0x3c) + -1;
  }
  iVar1 = *(int *)((long)param_1 + 0x3c);
  iVar4 = iVar1 + 1;
  lVar2 = *param_1;
  iVar3 = iVar4;
  if ((int)(uint)*(byte *)(lVar2 + 0x73) <= iVar1) {
    if (0xf8 < iVar1) {
      luaX_syntaxerror(param_1[3],"function or expression too complex");
      lVar2 = *param_1;
      iVar3 = *(int *)((long)param_1 + 0x3c) + 1;
    }
    *(char *)(lVar2 + 0x73) = (char)iVar4;
  }
  *(int *)((long)param_1 + 0x3c) = iVar3;
  luaK_exp2reg(param_1,param_2,iVar3 + -1);
  return;
}



// ======== luaK_exp2reg @ 0064b0a0 ========

void luaK_exp2reg(long *param_1,int *param_2,int param_3)

{
  undefined4 uVar1;
  long lVar2;
  int iVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  uint uVar6;
  uint uVar7;
  int iVar8;
  uint uVar9;
  uint uVar10;
  long lVar11;
  uint uVar12;
  uint uVar13;
  
  discharge2reg();
  if ((*param_2 == 9) && (iVar8 = param_2[1], iVar8 != -1)) {
    if (param_2[3] != 0xffffffff) {
      lVar2 = *(long *)(*param_1 + 0x18);
      uVar7 = param_2[3];
      do {
        uVar13 = uVar7;
        lVar11 = (long)(int)uVar13;
        uVar12 = *(uint *)(lVar2 + lVar11 * 4);
        uVar6 = uVar12 >> 6 & 0x3ffff;
        uVar7 = (uVar13 - 0x1fffe) + uVar6;
        if (uVar6 == 0x1fffe) {
          uVar7 = 0xffffffff;
        }
      } while (uVar7 != 0xffffffff);
      uVar13 = iVar8 + ~uVar13;
      uVar7 = -uVar13;
      if ((int)-uVar13 < 0) {
        uVar7 = uVar13;
      }
      if (0x1ffff < uVar7) {
        luaX_syntaxerror(param_1[3],"control structure too long");
        uVar12 = *(uint *)(lVar2 + lVar11 * 4);
      }
      *(uint *)(lVar2 + lVar11 * 4) = uVar13 * 0x40 + 0x7fffc0 & 0xffffc0 | uVar12 & 0xff00003f;
      goto LAB_0064b164;
    }
    param_2[3] = iVar8;
    iVar8 = param_2[3];
    iVar3 = param_2[4];
    if (iVar8 == iVar3) goto LAB_0064b481;
  }
  else {
LAB_0064b164:
    iVar8 = param_2[3];
    iVar3 = param_2[4];
    if (iVar8 == iVar3) goto LAB_0064b481;
  }
  if (iVar8 != -1) {
    lVar2 = *(long *)(*param_1 + 0x18);
    do {
      lVar11 = (long)iVar8;
      if ((iVar8 < 1) ||
         (uVar7 = *(uint *)(lVar2 + -4 + lVar11 * 4), -1 < (char)luaP_opmodes[uVar7 & 0x3f])) {
        uVar7 = *(uint *)(lVar2 + lVar11 * 4);
      }
      if ((uVar7 & 0x7fff) != 0x58) goto LAB_0064b255;
      uVar7 = *(uint *)(lVar2 + lVar11 * 4) >> 6 & 0x3ffff;
      iVar8 = iVar8 + -0x1fffe + uVar7;
      if (uVar7 == 0x1fffe) {
        iVar8 = -1;
      }
    } while (iVar8 != -1);
  }
  uVar4 = 0xffffffff;
  if (iVar3 != -1) {
    lVar2 = *(long *)(*param_1 + 0x18);
    do {
      lVar11 = (long)iVar3;
      if ((iVar3 < 1) ||
         (uVar7 = *(uint *)(lVar2 + -4 + lVar11 * 4), -1 < (char)luaP_opmodes[uVar7 & 0x3f])) {
        uVar7 = *(uint *)(lVar2 + lVar11 * 4);
      }
      if ((uVar7 & 0x7fff) != 0x18) goto LAB_0064b255;
      uVar7 = *(uint *)(lVar2 + lVar11 * 4) >> 6 & 0x3ffff;
      iVar3 = iVar3 + -0x1fffe + uVar7;
      if (uVar7 == 0x1fffe) {
        iVar3 = -1;
      }
    } while (iVar3 != -1);
  }
  uVar5 = 0xffffffff;
  goto LAB_0064b440;
LAB_0064b255:
  uVar7 = 0xffffffff;
  if (*param_2 != 9) {
    uVar12 = *(uint *)(param_1 + 7);
    *(undefined4 *)(param_1 + 7) = 0xffffffff;
    uVar13 = luaK_code(param_1,0x7fff94,*(undefined4 *)(param_1[3] + 8));
    uVar7 = uVar13;
    if ((uVar12 != 0xffffffff) && (uVar7 = uVar12, uVar13 != 0xffffffff)) {
      lVar2 = *(long *)(*param_1 + 0x18);
      uVar7 = uVar13;
      do {
        uVar10 = uVar7;
        lVar11 = (long)(int)uVar10;
        uVar6 = *(uint *)(lVar2 + lVar11 * 4);
        uVar9 = uVar6 >> 6 & 0x3ffff;
        uVar7 = (uVar10 - 0x1fffe) + uVar9;
        if (uVar9 == 0x1fffe) {
          uVar7 = 0xffffffff;
        }
      } while (uVar7 != 0xffffffff);
      uVar12 = uVar12 + ~uVar10;
      uVar7 = -uVar12;
      if ((int)-uVar12 < 0) {
        uVar7 = uVar12;
      }
      if (0x1ffff < uVar7) {
        luaX_syntaxerror(param_1[3],"control structure too long");
        uVar6 = *(uint *)(lVar2 + lVar11 * 4);
      }
      *(uint *)(lVar2 + lVar11 * 4) = uVar12 * 0x40 + 0x7fffc0 & 0xffffc0 | uVar6 & 0xff00003f;
      uVar7 = uVar13;
    }
  }
  *(int *)((long)param_1 + 0x34) = (int)param_1[6];
  uVar4 = luaK_code(param_1,param_3 * 0x1000000 + 0x42,*(undefined4 *)(param_1[3] + 8));
  *(int *)((long)param_1 + 0x34) = (int)param_1[6];
  uVar5 = luaK_code(param_1,param_3 * 0x1000000 | 0x8002,*(undefined4 *)(param_1[3] + 8));
  *(int *)((long)param_1 + 0x34) = (int)param_1[6];
  if (uVar7 != 0xffffffff) {
    if (*(uint *)(param_1 + 7) == 0xffffffff) {
      *(uint *)(param_1 + 7) = uVar7;
    }
    else {
      lVar2 = *(long *)(*param_1 + 0x18);
      uVar12 = *(uint *)(param_1 + 7);
      do {
        uVar6 = uVar12;
        lVar11 = (long)(int)uVar6;
        uVar13 = *(uint *)(lVar2 + lVar11 * 4);
        uVar10 = uVar13 >> 6 & 0x3ffff;
        uVar12 = (uVar6 - 0x1fffe) + uVar10;
        if (uVar10 == 0x1fffe) {
          uVar12 = 0xffffffff;
        }
      } while (uVar12 != 0xffffffff);
      uVar7 = uVar7 + ~uVar6;
      uVar12 = -uVar7;
      if ((int)-uVar7 < 0) {
        uVar12 = uVar7;
      }
      if (0x1ffff < uVar12) {
        luaX_syntaxerror(param_1[3],"control structure too long");
        uVar13 = *(uint *)(lVar2 + lVar11 * 4);
      }
      *(uint *)(lVar2 + lVar11 * 4) = uVar7 * 0x40 + 0x7fffc0 & 0xffffc0 | uVar13 & 0xff00003f;
    }
  }
LAB_0064b440:
  uVar1 = (undefined4)param_1[6];
  *(undefined4 *)((long)param_1 + 0x34) = uVar1;
  luaK_patchlistaux(param_1,param_2[4],uVar4,0xff,uVar1,param_3,uVar4);
  luaK_patchlistaux(param_1,param_2[3],uVar1,param_3,uVar5,0xff,uVar5);
LAB_0064b481:
  param_2[3] = -1;
  param_2[4] = -1;
  param_2[1] = param_3;
  *param_2 = 0xb;
  return;
}



// ======== luaK_exp2anyreg @ 0064b4b0 ========

int luaK_exp2anyreg(long *param_1,int *param_2)

{
  int iVar1;
  long lVar2;
  int iVar3;
  int iVar4;
  
  luaK_dischargevars();
  if (*param_2 == 0xb) {
    if (param_2[3] == param_2[4]) goto LAB_0064b55b;
    if (*(int *)((long)param_1 + 0x4c) <= param_2[1]) {
      luaK_exp2reg(param_1,param_2);
      goto LAB_0064b55b;
    }
  }
  luaK_dischargevars(param_1,param_2);
  if (((*param_2 == 0xb) && (param_2[1] < 0xfa)) && (*(int *)((long)param_1 + 0x4c) <= param_2[1]))
  {
    *(int *)((long)param_1 + 0x3c) = *(int *)((long)param_1 + 0x3c) + -1;
  }
  iVar1 = *(int *)((long)param_1 + 0x3c);
  iVar4 = iVar1 + 1;
  lVar2 = *param_1;
  iVar3 = iVar4;
  if ((int)(uint)*(byte *)(lVar2 + 0x73) <= iVar1) {
    if (0xf8 < iVar1) {
      luaX_syntaxerror(param_1[3],"function or expression too complex");
      lVar2 = *param_1;
      iVar3 = *(int *)((long)param_1 + 0x3c) + 1;
    }
    *(char *)(lVar2 + 0x73) = (char)iVar4;
  }
  *(int *)((long)param_1 + 0x3c) = iVar3;
  luaK_exp2reg(param_1,param_2,iVar3 + -1);
LAB_0064b55b:
  return param_2[1];
}



// ======== luaK_exp2val @ 0064b570 ========

void luaK_exp2val(undefined8 param_1,long param_2)

{
  if (*(int *)(param_2 + 0xc) == *(int *)(param_2 + 0x10)) {
    luaK_dischargevars();
    return;
  }
  luaK_exp2anyreg();
  return;
}



// ======== luaK_exp2RK @ 0064b590 ========

ulong luaK_exp2RK(long param_1,int *param_2)

{
  int iVar1;
  ulong uVar2;
  undefined4 local_30 [2];
  undefined8 local_28;
  undefined4 local_20 [4];
  
  if (param_2[3] == param_2[4]) {
    luaK_dischargevars(param_1,param_2);
    iVar1 = *param_2;
  }
  else {
    luaK_exp2anyreg(param_1,param_2);
    iVar1 = *param_2;
  }
  if (iVar1 == 4) {
    iVar1 = param_2[1];
    if (0x105 < iVar1) goto LAB_0064b625;
  }
  else {
    if ((iVar1 != 1) || (0x105 < *(int *)(param_1 + 0x40))) {
LAB_0064b625:
      uVar2 = luaK_exp2anyreg(param_1,param_2);
      return uVar2;
    }
    local_20[0] = 0;
    local_30[0] = 5;
    local_28 = *(undefined8 *)(param_1 + 8);
    iVar1 = addk(param_1,local_30,local_20);
    param_2[1] = iVar1;
    *param_2 = 4;
  }
  return (ulong)(iVar1 + 0xfa);
}



// ======== luaK_storevar @ 0064b640 ========

void luaK_storevar(long param_1,undefined4 *param_2,int *param_3)

{
  int iVar1;
  uint uVar2;
  
  switch(*param_2) {
  case 5:
    if (((*param_3 == 0xb) && (param_3[1] < 0xfa)) && (*(int *)(param_1 + 0x4c) <= param_3[1])) {
      *(int *)(param_1 + 0x3c) = *(int *)(param_1 + 0x3c) + -1;
    }
    luaK_exp2reg(param_1,param_3,param_2[1]);
    return;
  case 6:
    iVar1 = luaK_exp2anyreg(param_1,param_3);
    uVar2 = param_2[1] << 0xf | iVar1 << 0x18 | 8;
    break;
  case 7:
    iVar1 = luaK_exp2anyreg(param_1,param_3);
    uVar2 = param_2[1] << 6 | iVar1 << 0x18 | 7;
    break;
  case 8:
    iVar1 = luaK_exp2RK(param_1,param_3);
    uVar2 = iVar1 << 6 | param_2[1] << 0x18 | param_2[2] << 0xf | 9;
    break;
  default:
    goto switchD_0064b65c_default;
  }
  luaK_code(param_1,uVar2,*(undefined4 *)(*(long *)(param_1 + 0x18) + 8));
switchD_0064b65c_default:
  if (((*param_3 == 0xb) && (param_3[1] < 0xfa)) && (*(int *)(param_1 + 0x4c) <= param_3[1])) {
    *(int *)(param_1 + 0x3c) = *(int *)(param_1 + 0x3c) + -1;
  }
  return;
}



// ======== luaK_self @ 0064b720 ========

void luaK_self(long *param_1,int *param_2,int *param_3)

{
  int iVar1;
  int iVar2;
  long lVar3;
  int iVar4;
  
  luaK_exp2anyreg();
  if (((*param_2 == 0xb) && (param_2[1] < 0xfa)) && (*(int *)((long)param_1 + 0x4c) <= param_2[1]))
  {
    *(int *)((long)param_1 + 0x3c) = *(int *)((long)param_1 + 0x3c) + -1;
  }
  iVar1 = *(int *)((long)param_1 + 0x3c);
  iVar2 = iVar1 + 2;
  lVar3 = *param_1;
  iVar4 = iVar2;
  if ((int)(uint)*(byte *)(lVar3 + 0x73) < iVar2) {
    if (0xf7 < iVar1) {
      luaX_syntaxerror(param_1[3],"function or expression too complex");
      lVar3 = *param_1;
      iVar4 = *(int *)((long)param_1 + 0x3c) + 2;
    }
    *(char *)(lVar3 + 0x73) = (char)iVar2;
  }
  *(int *)((long)param_1 + 0x3c) = iVar4;
  iVar2 = param_2[1];
  iVar4 = luaK_exp2RK(param_1,param_3);
  luaK_code(param_1,iVar2 << 0xf | iVar1 << 0x18 | iVar4 << 6 | 0xb,*(undefined4 *)(param_1[3] + 8))
  ;
  if (((*param_3 == 0xb) && (param_3[1] < 0xfa)) && (*(int *)((long)param_1 + 0x4c) <= param_3[1]))
  {
    *(int *)((long)param_1 + 0x3c) = *(int *)((long)param_1 + 0x3c) + -1;
  }
  param_2[1] = iVar1;
  *param_2 = 0xb;
  return;
}



// ======== luaK_goiftrue @ 0064b7f0 ========

void luaK_goiftrue(long *param_1,undefined4 *param_2)

{
  long lVar1;
  uint uVar2;
  uint uVar3;
  uint *puVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  long lVar8;
  uint uVar9;
  
  luaK_dischargevars();
  switch(*param_2) {
  case 2:
  case 4:
    goto switchD_0064b816_caseD_2;
  case 3:
    uVar9 = *(uint *)(param_1 + 7);
    *(undefined4 *)(param_1 + 7) = 0xffffffff;
    uVar3 = luaK_code(param_1,0x7fff94,*(undefined4 *)(param_1[3] + 8));
    if (uVar9 == 0xffffffff) break;
    if (uVar3 != 0xffffffff) {
      lVar1 = *(long *)(*param_1 + 0x18);
      uVar6 = uVar3;
      do {
        uVar7 = uVar6;
        lVar8 = (long)(int)uVar7;
        uVar2 = *(uint *)(lVar1 + lVar8 * 4);
        uVar5 = uVar2 >> 6 & 0x3ffff;
        uVar6 = (uVar7 - 0x1fffe) + uVar5;
        if (uVar5 == 0x1fffe) {
          uVar6 = 0xffffffff;
        }
      } while (uVar6 != 0xffffffff);
      uVar9 = uVar9 + ~uVar7;
      uVar6 = -uVar9;
      if ((int)-uVar9 < 0) {
        uVar6 = uVar9;
      }
      if (0x1ffff < uVar6) {
        luaX_syntaxerror(param_1[3],"control structure too long");
        uVar2 = *(uint *)(lVar1 + lVar8 * 4);
      }
      *(uint *)(lVar1 + lVar8 * 4) = uVar9 * 0x40 + 0x7fffc0 & 0xffffc0 | uVar2 & 0xff00003f;
      uVar9 = uVar3;
      if (uVar3 == 0xffffffff) {
        return;
      }
    }
    goto LAB_0064b95f;
  default:
    uVar3 = jumponcond(param_1,param_2,0);
    break;
  case 9:
    puVar4 = (uint *)((long)(int)param_2[1] * 4 + *(long *)(*param_1 + 0x18));
    if (((long)(int)param_2[1] < 1) || (uVar3 = puVar4[-1], -1 < (char)luaP_opmodes[uVar3 & 0x3f]))
    {
      uVar3 = *puVar4;
    }
    else {
      puVar4 = puVar4 + -1;
    }
    *puVar4 = (uint)(uVar3 < 0x1000000) << 0x18 | uVar3 & 0xffffff;
    uVar3 = param_2[1];
  }
  uVar9 = uVar3;
  if (uVar3 != 0xffffffff) {
LAB_0064b95f:
    if (param_2[4] == 0xffffffff) {
      param_2[4] = uVar9;
    }
    else {
      lVar1 = *(long *)(*param_1 + 0x18);
      uVar3 = param_2[4];
      do {
        uVar2 = uVar3;
        lVar8 = (long)(int)uVar2;
        uVar6 = *(uint *)(lVar1 + lVar8 * 4);
        uVar7 = uVar6 >> 6 & 0x3ffff;
        uVar3 = (uVar2 - 0x1fffe) + uVar7;
        if (uVar7 == 0x1fffe) {
          uVar3 = 0xffffffff;
        }
      } while (uVar3 != 0xffffffff);
      uVar9 = uVar9 + ~uVar2;
      uVar3 = -uVar9;
      if ((int)-uVar9 < 0) {
        uVar3 = uVar9;
      }
      if (0x1ffff < uVar3) {
        luaX_syntaxerror(param_1[3],"control structure too long");
        uVar6 = *(uint *)(lVar1 + lVar8 * 4);
      }
      *(uint *)(lVar1 + lVar8 * 4) = uVar9 * 0x40 + 0x7fffc0 & 0xffffc0 | uVar6 & 0xff00003f;
    }
  }
switchD_0064b816_caseD_2:
  return;
}



// ======== jumponcond @ 0064ba10 ========

void jumponcond(long *param_1,int *param_2,uint param_3)

{
  int iVar1;
  long lVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  
  if (*param_2 != 0xb) {
    if ((*param_2 == 10) &&
       (uVar3 = *(uint *)(*(long *)(*param_1 + 0x18) + (long)param_2[1] * 4), (uVar3 & 0x3f) == 0x12
       )) {
      *(int *)(param_1 + 6) = (int)param_1[6] + -1;
      uVar3 = uVar3 >> 0xf & 0x1ff;
      param_3 = (uint)(param_3 == 0);
      goto LAB_0064baed;
    }
    iVar1 = *(int *)((long)param_1 + 0x3c);
    iVar5 = iVar1 + 1;
    lVar2 = *param_1;
    iVar4 = iVar5;
    if ((int)(uint)*(byte *)(lVar2 + 0x73) <= iVar1) {
      if (0xf8 < iVar1) {
        luaX_syntaxerror(param_1[3],"function or expression too complex");
        lVar2 = *param_1;
        iVar4 = *(int *)((long)param_1 + 0x3c) + 1;
      }
      *(char *)(lVar2 + 0x73) = (char)iVar5;
    }
    *(int *)((long)param_1 + 0x3c) = iVar4;
    discharge2reg(param_1,param_2,iVar4 + -1);
    if (*param_2 != 0xb) {
      uVar3 = param_2[1];
      goto LAB_0064baed;
    }
  }
  uVar3 = param_2[1];
  if (((int)uVar3 < 0xfa) && (*(int *)((long)param_1 + 0x4c) <= (int)uVar3)) {
    *(int *)((long)param_1 + 0x3c) = *(int *)((long)param_1 + 0x3c) + -1;
  }
LAB_0064baed:
  luaK_condjump(param_1,0x18,0xff,uVar3,param_3);
  return;
}



// ======== luaK_goiffalse @ 0064bb00 ========

void luaK_goiffalse(long *param_1,undefined4 *param_2)

{
  long lVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  long lVar7;
  uint uVar8;
  
  luaK_dischargevars();
  switch(*param_2) {
  case 1:
  case 3:
    goto switchD_0064bb26_caseD_1;
  case 2:
    uVar8 = *(uint *)(param_1 + 7);
    *(undefined4 *)(param_1 + 7) = 0xffffffff;
    uVar3 = luaK_code(param_1,0x7fff94,*(undefined4 *)(param_1[3] + 8));
    if (uVar8 == 0xffffffff) break;
    if (uVar3 != 0xffffffff) {
      lVar1 = *(long *)(*param_1 + 0x18);
      uVar5 = uVar3;
      do {
        uVar6 = uVar5;
        lVar7 = (long)(int)uVar6;
        uVar2 = *(uint *)(lVar1 + lVar7 * 4);
        uVar4 = uVar2 >> 6 & 0x3ffff;
        uVar5 = (uVar6 - 0x1fffe) + uVar4;
        if (uVar4 == 0x1fffe) {
          uVar5 = 0xffffffff;
        }
      } while (uVar5 != 0xffffffff);
      uVar8 = uVar8 + ~uVar6;
      uVar5 = -uVar8;
      if ((int)-uVar8 < 0) {
        uVar5 = uVar8;
      }
      if (0x1ffff < uVar5) {
        luaX_syntaxerror(param_1[3],"control structure too long");
        uVar2 = *(uint *)(lVar1 + lVar7 * 4);
      }
      *(uint *)(lVar1 + lVar7 * 4) = uVar8 * 0x40 + 0x7fffc0 & 0xffffc0 | uVar2 & 0xff00003f;
      uVar8 = uVar3;
      if (uVar3 == 0xffffffff) {
        return;
      }
    }
    goto LAB_0064bc21;
  default:
    uVar3 = jumponcond(param_1,param_2,1);
    break;
  case 9:
    uVar3 = param_2[1];
  }
  uVar8 = uVar3;
  if (uVar3 != 0xffffffff) {
LAB_0064bc21:
    if (param_2[3] == 0xffffffff) {
      param_2[3] = uVar8;
    }
    else {
      lVar1 = *(long *)(*param_1 + 0x18);
      uVar3 = param_2[3];
      do {
        uVar2 = uVar3;
        lVar7 = (long)(int)uVar2;
        uVar5 = *(uint *)(lVar1 + lVar7 * 4);
        uVar6 = uVar5 >> 6 & 0x3ffff;
        uVar3 = (uVar2 - 0x1fffe) + uVar6;
        if (uVar6 == 0x1fffe) {
          uVar3 = 0xffffffff;
        }
      } while (uVar3 != 0xffffffff);
      uVar8 = uVar8 + ~uVar2;
      uVar3 = -uVar8;
      if ((int)-uVar8 < 0) {
        uVar3 = uVar8;
      }
      if (0x1ffff < uVar3) {
        luaX_syntaxerror(param_1[3],"control structure too long");
        uVar5 = *(uint *)(lVar1 + lVar7 * 4);
      }
      *(uint *)(lVar1 + lVar7 * 4) = uVar8 * 0x40 + 0x7fffc0 & 0xffffc0 | uVar5 & 0xff00003f;
    }
  }
switchD_0064bb26_caseD_1:
  return;
}



// ======== luaK_indexed @ 0064bcd0 ========

void luaK_indexed(undefined8 param_1,undefined4 *param_2,undefined8 param_3)

{
  undefined4 uVar1;
  
  uVar1 = luaK_exp2RK(param_1,param_3);
  param_2[2] = uVar1;
  *param_2 = 8;
  return;
}



// ======== luaK_prefix @ 0064bcf0 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void luaK_prefix(long *param_1,int param_2,int *param_3)

{
  ulong *puVar1;
  undefined8 uVar2;
  int iVar3;
  uint *puVar4;
  long lVar5;
  int iVar6;
  uint uVar7;
  int iVar8;
  undefined4 local_28 [2];
  undefined8 local_20;
  
  if (param_2 == 0) {
    if (param_3[3] == param_3[4]) {
      luaK_dischargevars(param_1,param_3);
      iVar3 = *param_3;
    }
    else {
      luaK_exp2anyreg(param_1,param_3);
      iVar3 = *param_3;
    }
    if (iVar3 == 4) {
      if (*(int *)(*(long *)(*param_1 + 0x10) + (long)param_3[1] * 0x10) == 3) {
        uVar2 = *(undefined8 *)(*(long *)(*param_1 + 0x10) + 8 + (long)param_3[1] * 0x10);
        local_20 = CONCAT44((uint)((ulong)uVar2 >> 0x20) ^ _UNK_0066c304,(uint)uVar2 ^ _DAT_0066c300
                           );
        local_28[0] = 3;
        iVar3 = addk(param_1,local_28);
        param_3[1] = iVar3;
        return;
      }
    }
    luaK_exp2anyreg(param_1,param_3);
    iVar3 = param_3[1];
    if (((*param_3 == 0xb) && (iVar3 < 0xfa)) && (*(int *)((long)param_1 + 0x4c) <= iVar3)) {
      *(int *)((long)param_1 + 0x3c) = *(int *)((long)param_1 + 0x3c) + -1;
    }
    iVar3 = luaK_code(param_1,iVar3 << 0xf | 0x11,*(undefined4 *)(param_1[3] + 8));
    param_3[1] = iVar3;
    *param_3 = 10;
    return;
  }
  luaK_dischargevars(param_1,param_3);
  switch(*param_3) {
  case 1:
  case 3:
    *param_3 = 2;
    break;
  case 2:
  case 4:
    *param_3 = 3;
    break;
  case 9:
    puVar4 = (uint *)((long)param_3[1] * 4 + *(long *)(*param_1 + 0x18));
    if (((long)param_3[1] < 1) || (uVar7 = puVar4[-1], -1 < (char)luaP_opmodes[uVar7 & 0x3f])) {
      uVar7 = *puVar4;
    }
    else {
      puVar4 = puVar4 + -1;
    }
    *puVar4 = (uint)(uVar7 < 0x1000000) << 0x18 | uVar7 & 0xffffff;
    break;
  case 10:
    iVar3 = *(int *)((long)param_1 + 0x3c);
    iVar8 = iVar3 + 1;
    lVar5 = *param_1;
    iVar6 = iVar8;
    if ((int)(uint)*(byte *)(lVar5 + 0x73) <= iVar3) {
      if (0xf8 < iVar3) {
        luaX_syntaxerror(param_1[3],"function or expression too complex");
        lVar5 = *param_1;
        iVar6 = *(int *)((long)param_1 + 0x3c) + 1;
      }
      *(char *)(lVar5 + 0x73) = (char)iVar8;
    }
    *(int *)((long)param_1 + 0x3c) = iVar6;
    discharge2reg(param_1,param_3,iVar6 + -1);
    if (*param_3 == 0xb) goto switchD_0064bd1b_caseD_b;
    iVar3 = param_3[1];
    goto LAB_0064be91;
  case 0xb:
switchD_0064bd1b_caseD_b:
    iVar3 = param_3[1];
    if ((iVar3 < 0xfa) && (*(int *)((long)param_1 + 0x4c) <= iVar3)) {
      *(int *)((long)param_1 + 0x3c) = *(int *)((long)param_1 + 0x3c) + -1;
    }
LAB_0064be91:
    iVar3 = luaK_code(param_1,iVar3 << 0xf | 0x12,*(undefined4 *)(param_1[3] + 8));
    param_3[1] = iVar3;
    *param_3 = 10;
  }
  puVar1 = (ulong *)(param_3 + 3);
  *puVar1 = *puVar1 << 0x20 | *puVar1 >> 0x20;
  return;
}



// ======== luaK_infix @ 0064bee0 ========

void luaK_infix(long *param_1,int param_2,int *param_3)

{
  int iVar1;
  uint uVar2;
  long lVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  long lVar7;
  int iVar8;
  uint uVar9;
  
  if (param_2 == 5) {
    luaK_dischargevars(param_1,param_3);
    if (((*param_3 == 0xb) && (param_3[1] < 0xfa)) && (*(int *)((long)param_1 + 0x4c) <= param_3[1])
       ) {
      *(int *)((long)param_1 + 0x3c) = *(int *)((long)param_1 + 0x3c) + -1;
    }
    iVar1 = *(int *)((long)param_1 + 0x3c);
    iVar8 = iVar1 + 1;
    lVar3 = *param_1;
    iVar4 = iVar8;
    if ((int)(uint)*(byte *)(lVar3 + 0x73) <= iVar1) {
      if (0xf8 < iVar1) {
        luaX_syntaxerror(param_1[3],"function or expression too complex");
        lVar3 = *param_1;
        iVar4 = *(int *)((long)param_1 + 0x3c) + 1;
      }
      *(char *)(lVar3 + 0x73) = (char)iVar8;
    }
    *(int *)((long)param_1 + 0x3c) = iVar4;
    luaK_exp2reg(param_1,param_3,iVar4 + -1);
    return;
  }
  if (param_2 == 0xd) {
    luaK_goiffalse(param_1,param_3);
    iVar1 = param_3[4];
    *(int *)((long)param_1 + 0x34) = (int)param_1[6];
    if (iVar1 != -1) {
      if (*(uint *)(param_1 + 7) == 0xffffffff) {
        *(int *)(param_1 + 7) = iVar1;
      }
      else {
        lVar3 = *(long *)(*param_1 + 0x18);
        uVar6 = *(uint *)(param_1 + 7);
        do {
          uVar9 = uVar6;
          lVar7 = (long)(int)uVar9;
          uVar2 = *(uint *)(lVar3 + lVar7 * 4);
          uVar5 = uVar2 >> 6 & 0x3ffff;
          uVar6 = (uVar9 - 0x1fffe) + uVar5;
          if (uVar5 == 0x1fffe) {
            uVar6 = 0xffffffff;
          }
        } while (uVar6 != 0xffffffff);
        uVar9 = iVar1 + ~uVar9;
        uVar6 = -uVar9;
        if ((int)-uVar9 < 0) {
          uVar6 = uVar9;
        }
        if (0x1ffff < uVar6) {
          luaX_syntaxerror(param_1[3],"control structure too long");
          uVar2 = *(uint *)(lVar3 + lVar7 * 4);
        }
        *(uint *)(lVar3 + lVar7 * 4) = uVar9 * 0x40 + 0x7fffc0 & 0xffffc0 | uVar2 & 0xff00003f;
      }
    }
    param_3[4] = -1;
  }
  else {
    if (param_2 != 0xc) {
      luaK_exp2RK(param_1,param_3);
      return;
    }
    luaK_goiftrue(param_1,param_3);
    iVar1 = param_3[3];
    *(int *)((long)param_1 + 0x34) = (int)param_1[6];
    if (iVar1 != -1) {
      if (*(uint *)(param_1 + 7) == 0xffffffff) {
        *(int *)(param_1 + 7) = iVar1;
        param_3[3] = -1;
        return;
      }
      lVar3 = *(long *)(*param_1 + 0x18);
      uVar6 = *(uint *)(param_1 + 7);
      do {
        uVar9 = uVar6;
        lVar7 = (long)(int)uVar9;
        uVar2 = *(uint *)(lVar3 + lVar7 * 4);
        uVar5 = uVar2 >> 6 & 0x3ffff;
        uVar6 = (uVar9 - 0x1fffe) + uVar5;
        if (uVar5 == 0x1fffe) {
          uVar6 = 0xffffffff;
        }
      } while (uVar6 != 0xffffffff);
      uVar9 = iVar1 + ~uVar9;
      uVar6 = -uVar9;
      if ((int)-uVar9 < 0) {
        uVar6 = uVar9;
      }
      if (0x1ffff < uVar6) {
        luaX_syntaxerror(param_1[3],"control structure too long");
        uVar2 = *(uint *)(lVar3 + lVar7 * 4);
      }
      *(uint *)(lVar3 + lVar7 * 4) = uVar9 * 0x40 + 0x7fffc0 & 0xffffc0 | uVar2 & 0xff00003f;
    }
    param_3[3] = -1;
  }
  return;
}



// ======== luaK_posfix @ 0064c140 ========

void luaK_posfix(long *param_1,uint param_2,int *param_3,int *param_4)

{
  undefined8 uVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  long lVar5;
  uint uVar6;
  uint uVar7;
  int iVar8;
  long lVar9;
  uint uVar10;
  
  if (param_2 == 5) {
    if (param_4[3] == param_4[4]) {
      luaK_dischargevars(param_1,param_4);
      iVar4 = *param_4;
    }
    else {
      luaK_exp2anyreg(param_1,param_4);
      iVar4 = *param_4;
    }
    if (iVar4 == 10) {
      lVar5 = *(long *)(*param_1 + 0x18);
      lVar9 = (long)param_4[1];
      uVar7 = *(uint *)(lVar5 + lVar9 * 4);
      if ((uVar7 & 0x3f) == 0x13) {
        uVar2 = param_3[1];
        if (((*param_3 == 0xb) && ((int)uVar2 < 0xfa)) &&
           (*(int *)((long)param_1 + 0x4c) <= (int)uVar2)) {
          *(int *)((long)param_1 + 0x3c) = *(int *)((long)param_1 + 0x3c) + -1;
          uVar7 = *(uint *)(lVar5 + lVar9 * 4);
        }
        *(uint *)(lVar5 + lVar9 * 4) = (uVar2 & 0x1ff) << 0xf | uVar7 & 0xff007fff;
        *param_3 = 10;
        param_3[1] = param_4[1];
        return;
      }
    }
    luaK_dischargevars(param_1,param_4);
    if (((*param_4 == 0xb) && (param_4[1] < 0xfa)) && (*(int *)((long)param_1 + 0x4c) <= param_4[1])
       ) {
      *(int *)((long)param_1 + 0x3c) = *(int *)((long)param_1 + 0x3c) + -1;
    }
    iVar4 = *(int *)((long)param_1 + 0x3c);
    iVar8 = iVar4 + 1;
    lVar5 = *param_1;
    iVar3 = iVar8;
    if ((int)(uint)*(byte *)(lVar5 + 0x73) <= iVar4) {
      if (0xf8 < iVar4) {
        luaX_syntaxerror(param_1[3],"function or expression too complex");
        lVar5 = *param_1;
        iVar3 = *(int *)((long)param_1 + 0x3c) + 1;
      }
      *(char *)(lVar5 + 0x73) = (char)iVar8;
    }
    *(int *)((long)param_1 + 0x3c) = iVar3;
    luaK_exp2reg(param_1,param_4,iVar3 + -1);
    if (((*param_4 == 0xb) && (param_4[1] < 0xfa)) && (*(int *)((long)param_1 + 0x4c) <= param_4[1])
       ) {
      *(int *)((long)param_1 + 0x3c) = *(int *)((long)param_1 + 0x3c) + -1;
    }
    iVar4 = param_3[1];
    if (((*param_3 == 0xb) && (iVar4 < 0xfa)) && (*(int *)((long)param_1 + 0x4c) <= iVar4)) {
      *(int *)((long)param_1 + 0x3c) = *(int *)((long)param_1 + 0x3c) + -1;
    }
    iVar4 = luaK_code(param_1,param_4[1] << 6 | iVar4 << 0xf | 0x13,*(undefined4 *)(param_1[3] + 8))
    ;
    param_3[1] = iVar4;
    *param_3 = 10;
  }
  else if (param_2 == 0xd) {
    luaK_dischargevars(param_1,param_4);
    iVar4 = param_4[3];
    if (iVar4 != -1) {
      if (param_3[3] == 0xffffffff) {
        param_3[3] = iVar4;
      }
      else {
        lVar5 = *(long *)(*param_1 + 0x18);
        uVar7 = param_3[3];
        do {
          uVar10 = uVar7;
          lVar9 = (long)(int)uVar10;
          uVar2 = *(uint *)(lVar5 + lVar9 * 4);
          uVar6 = uVar2 >> 6 & 0x3ffff;
          uVar7 = (uVar10 - 0x1fffe) + uVar6;
          if (uVar6 == 0x1fffe) {
            uVar7 = 0xffffffff;
          }
        } while (uVar7 != 0xffffffff);
        uVar10 = iVar4 + ~uVar10;
        uVar7 = -uVar10;
        if ((int)-uVar10 < 0) {
          uVar7 = uVar10;
        }
        if (0x1ffff < uVar7) {
          luaX_syntaxerror(param_1[3],"control structure too long");
          uVar2 = *(uint *)(lVar5 + lVar9 * 4);
        }
        *(uint *)(lVar5 + lVar9 * 4) = uVar10 * 0x40 + 0x7fffc0 & 0xffffc0 | uVar2 & 0xff00003f;
      }
    }
    *param_3 = *param_4;
    param_3[1] = param_4[1];
    param_3[2] = param_4[2];
    param_3[4] = param_4[4];
  }
  else if (param_2 == 0xc) {
    luaK_dischargevars(param_1,param_4);
    iVar4 = param_4[4];
    if (iVar4 != -1) {
      if (param_3[4] == 0xffffffff) {
        param_3[4] = iVar4;
      }
      else {
        lVar5 = *(long *)(*param_1 + 0x18);
        uVar7 = param_3[4];
        do {
          uVar10 = uVar7;
          lVar9 = (long)(int)uVar10;
          uVar2 = *(uint *)(lVar5 + lVar9 * 4);
          uVar6 = uVar2 >> 6 & 0x3ffff;
          uVar7 = (uVar10 - 0x1fffe) + uVar6;
          if (uVar6 == 0x1fffe) {
            uVar7 = 0xffffffff;
          }
        } while (uVar7 != 0xffffffff);
        uVar10 = iVar4 + ~uVar10;
        uVar7 = -uVar10;
        if ((int)-uVar10 < 0) {
          uVar7 = uVar10;
        }
        if (0x1ffff < uVar7) {
          luaX_syntaxerror(param_1[3],"control structure too long");
          uVar2 = *(uint *)(lVar5 + lVar9 * 4);
        }
        *(uint *)(lVar5 + lVar9 * 4) = uVar10 * 0x40 + 0x7fffc0 & 0xffffc0 | uVar2 & 0xff00003f;
      }
    }
    uVar1 = *(undefined8 *)(param_4 + 2);
    *(undefined8 *)param_3 = *(undefined8 *)param_4;
    *(undefined8 *)(param_3 + 2) = uVar1;
  }
  else {
    iVar3 = luaK_exp2RK(param_1,param_3);
    iVar4 = luaK_exp2RK(param_1,param_4);
    if (((*param_4 == 0xb) && (param_4[1] < 0xfa)) && (*(int *)((long)param_1 + 0x4c) <= param_4[1])
       ) {
      *(int *)((long)param_1 + 0x3c) = *(int *)((long)param_1 + 0x3c) + -1;
    }
    if (((*param_3 == 0xb) && (param_3[1] < 0xfa)) && (*(int *)((long)param_1 + 0x4c) <= param_3[1])
       ) {
      *(int *)((long)param_1 + 0x3c) = *(int *)((long)param_1 + 0x3c) + -1;
    }
    if (param_2 < 5) {
      iVar4 = luaK_code(param_1,iVar4 << 6 | iVar3 << 0xf | param_2 + 0xc,
                        *(undefined4 *)(param_1[3] + 8));
      iVar3 = 10;
    }
    else {
      iVar8 = iVar3;
      if (9 < param_2) {
        iVar8 = iVar4;
        iVar4 = iVar3;
      }
      iVar4 = luaK_condjump(param_1,*(undefined4 *)(codebinop_ops + (ulong)(param_2 - 6) * 4),
                            param_2 != 6,iVar8,iVar4);
      iVar3 = 9;
    }
    param_3[1] = iVar4;
    *param_3 = iVar3;
  }
  return;
}



// ======== luaK_fixline @ 0064c540 ========

void luaK_fixline(long *param_1,undefined4 param_2)

{
  *(undefined4 *)(*(long *)(*param_1 + 0x28) + -4 + (long)(int)param_1[6] * 4) = param_2;
  return;
}



// ======== luaK_code @ 0064c550 ========

void luaK_code(long *param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 uVar1;
  long lVar2;
  long lVar3;
  int iVar4;
  
  lVar2 = *param_1;
  uVar1 = (undefined4)param_1[6];
  luaK_patchlistaux(param_1,(int)param_1[7],uVar1,0xff,uVar1,0xff,uVar1);
  *(undefined4 *)(param_1 + 7) = 0xffffffff;
  iVar4 = (int)param_1[6];
  if (iVar4 < *(int *)(lVar2 + 0x50)) {
    lVar3 = *(long *)(lVar2 + 0x18);
  }
  else {
    lVar3 = luaM_growaux(param_1[4],*(undefined8 *)(lVar2 + 0x18),lVar2 + 0x50,4,0x7ffffffd,
                         "code size overflow");
    *(long *)(lVar2 + 0x18) = lVar3;
    iVar4 = (int)param_1[6];
  }
  *(undefined4 *)(lVar3 + (long)iVar4 * 4) = param_2;
  iVar4 = (int)param_1[6];
  if (iVar4 < *(int *)(lVar2 + 0x54)) {
    lVar3 = *(long *)(lVar2 + 0x28);
  }
  else {
    lVar3 = luaM_growaux(param_1[4],*(undefined8 *)(lVar2 + 0x28),lVar2 + 0x54,4,0x7ffffffd,
                         "code size overflow");
    *(long *)(lVar2 + 0x28) = lVar3;
    iVar4 = (int)param_1[6];
  }
  *(undefined4 *)(lVar3 + (long)iVar4 * 4) = param_3;
  *(int *)(param_1 + 6) = (int)param_1[6] + 1;
  return;
}



// ======== discharge2reg @ 0064c620 ========

void discharge2reg(long *param_1,int *param_2,uint param_3)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  
  luaK_dischargevars();
  switch(*param_2) {
  case 1:
    iVar1 = (int)param_1[6];
    if (*(int *)((long)param_1 + 0x34) < iVar1) {
      uVar3 = *(uint *)(*(long *)(*param_1 + 0x18) + -4 + (long)iVar1 * 4);
      if ((((uVar3 & 0x3f) == 3) && ((int)(uVar3 >> 0x18) <= (int)param_3)) &&
         (uVar2 = uVar3 >> 0xf & 0x1ff, (int)param_3 <= (int)(uVar2 + 1))) {
        if ((int)uVar2 < (int)param_3) {
          *(uint *)(*(long *)(*param_1 + 0x18) + -4 + (long)iVar1 * 4) =
               (param_3 & 0x1ff) << 0xf | uVar3 & 0xff007fff;
        }
        goto LAB_0064c721;
      }
    }
    uVar3 = param_3 << 0xf | param_3 << 0x18 | 3;
    break;
  case 2:
  case 3:
    uVar3 = 2;
    if (*param_2 == 2) {
      uVar3 = 0x8000;
    }
    uVar3 = uVar3 | param_3 << 0x18 | 2;
    break;
  case 4:
    uVar3 = param_2[1] << 6 | param_3 << 0x18 | 1;
    break;
  default:
    goto switchD_0064c640_caseD_5;
  case 10:
    *(char *)(*(long *)(*param_1 + 0x18) + 3 + (long)param_2[1] * 4) = (char)param_3;
    goto LAB_0064c721;
  case 0xb:
    if (param_2[1] != param_3) {
      uVar3 = param_2[1] << 0xf | param_3 << 0x18;
      break;
    }
    goto LAB_0064c721;
  }
  luaK_code(param_1,uVar3,*(undefined4 *)(param_1[3] + 8));
LAB_0064c721:
  param_2[1] = param_3;
  *param_2 = 0xb;
switchD_0064c640_caseD_5:
  return;
}



// ======== luaK_condjump @ 0064c730 ========

uint luaK_condjump(long *param_1,uint param_2,int param_3,int param_4,int param_5)

{
  long lVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  long lVar7;
  uint uVar8;
  
  luaK_code(param_1,param_2 | param_3 << 0x18 | param_5 << 6 | param_4 << 0xf,
            *(undefined4 *)(param_1[3] + 8));
  uVar8 = *(uint *)(param_1 + 7);
  *(undefined4 *)(param_1 + 7) = 0xffffffff;
  uVar2 = luaK_code(param_1,0x7fff94,*(undefined4 *)(param_1[3] + 8));
  uVar6 = uVar2;
  if ((uVar8 != 0xffffffff) && (uVar6 = uVar8, uVar2 != 0xffffffff)) {
    lVar1 = *(long *)(*param_1 + 0x18);
    uVar6 = uVar2;
    do {
      uVar4 = uVar6;
      lVar7 = (long)(int)uVar4;
      uVar3 = *(uint *)(lVar1 + lVar7 * 4);
      uVar5 = uVar3 >> 6 & 0x3ffff;
      uVar6 = (uVar4 - 0x1fffe) + uVar5;
      if (uVar5 == 0x1fffe) {
        uVar6 = 0xffffffff;
      }
    } while (uVar6 != 0xffffffff);
    uVar8 = uVar8 + ~uVar4;
    uVar6 = -uVar8;
    if ((int)-uVar8 < 0) {
      uVar6 = uVar8;
    }
    if (0x1ffff < uVar6) {
      luaX_syntaxerror(param_1[3],"control structure too long");
      uVar3 = *(uint *)(lVar1 + lVar7 * 4);
    }
    *(uint *)(lVar1 + lVar7 * 4) = uVar8 * 0x40 + 0x7fffc0 & 0xffffc0 | uVar3 & 0xff00003f;
    uVar6 = uVar2;
  }
  return uVar6;
}



// ======== luaX_init @ 0064c830 ========

void luaX_init(undefined8 param_1)

{
  long lVar1;
  
  lVar1 = luaS_newlstr(param_1,&DAT_0066cac1,3);
  *(byte *)(lVar1 + 9) = *(byte *)(lVar1 + 9) | 0x10;
  *(undefined1 *)(lVar1 + 10) = 1;
  lVar1 = luaS_newlstr(param_1,"break",5);
  *(byte *)(lVar1 + 9) = *(byte *)(lVar1 + 9) | 0x10;
  *(undefined1 *)(lVar1 + 10) = 2;
  lVar1 = luaS_newlstr(param_1,"do",2);
  *(byte *)(lVar1 + 9) = *(byte *)(lVar1 + 9) | 0x10;
  *(undefined1 *)(lVar1 + 10) = 3;
  lVar1 = luaS_newlstr(param_1,&DAT_0066cac5,4);
  *(byte *)(lVar1 + 9) = *(byte *)(lVar1 + 9) | 0x10;
  *(undefined1 *)(lVar1 + 10) = 4;
  lVar1 = luaS_newlstr(param_1,&DAT_0066caca,6);
  *(byte *)(lVar1 + 9) = *(byte *)(lVar1 + 9) | 0x10;
  *(undefined1 *)(lVar1 + 10) = 5;
  lVar1 = luaS_newlstr(param_1,"end",3);
  *(byte *)(lVar1 + 9) = *(byte *)(lVar1 + 9) | 0x10;
  *(undefined1 *)(lVar1 + 10) = 6;
  lVar1 = luaS_newlstr(param_1,"false",5);
  *(byte *)(lVar1 + 9) = *(byte *)(lVar1 + 9) | 0x10;
  *(undefined1 *)(lVar1 + 10) = 7;
  lVar1 = luaS_newlstr(param_1,&DAT_0066cad1,3);
  *(byte *)(lVar1 + 9) = *(byte *)(lVar1 + 9) | 0x10;
  *(undefined1 *)(lVar1 + 10) = 8;
  lVar1 = luaS_newlstr(param_1,"function",8);
  *(byte *)(lVar1 + 9) = *(byte *)(lVar1 + 9) | 0x10;
  *(undefined1 *)(lVar1 + 10) = 9;
  lVar1 = luaS_newlstr(param_1,&DAT_0066cace,2);
  *(byte *)(lVar1 + 9) = *(byte *)(lVar1 + 9) | 0x10;
  *(undefined1 *)(lVar1 + 10) = 10;
  lVar1 = luaS_newlstr(param_1,"in",2);
  *(byte *)(lVar1 + 9) = *(byte *)(lVar1 + 9) | 0x10;
  *(undefined1 *)(lVar1 + 10) = 0xb;
  lVar1 = luaS_newlstr(param_1,"local",5);
  *(byte *)(lVar1 + 9) = *(byte *)(lVar1 + 9) | 0x10;
  *(undefined1 *)(lVar1 + 10) = 0xc;
  lVar1 = luaS_newlstr(param_1,"nil",3);
  *(byte *)(lVar1 + 9) = *(byte *)(lVar1 + 9) | 0x10;
  *(undefined1 *)(lVar1 + 10) = 0xd;
  lVar1 = luaS_newlstr(param_1,&DAT_0066cad5,3);
  *(byte *)(lVar1 + 9) = *(byte *)(lVar1 + 9) | 0x10;
  *(undefined1 *)(lVar1 + 10) = 0xe;
  lVar1 = luaS_newlstr(param_1,"or",2);
  *(byte *)(lVar1 + 9) = *(byte *)(lVar1 + 9) | 0x10;
  *(undefined1 *)(lVar1 + 10) = 0xf;
  lVar1 = luaS_newlstr(param_1,"repeat",6);
  *(byte *)(lVar1 + 9) = *(byte *)(lVar1 + 9) | 0x10;
  *(undefined1 *)(lVar1 + 10) = 0x10;
  lVar1 = luaS_newlstr(param_1,"return",6);
  *(byte *)(lVar1 + 9) = *(byte *)(lVar1 + 9) | 0x10;
  *(undefined1 *)(lVar1 + 10) = 0x11;
  lVar1 = luaS_newlstr(param_1,&DAT_0066cad9,4);
  *(byte *)(lVar1 + 9) = *(byte *)(lVar1 + 9) | 0x10;
  *(undefined1 *)(lVar1 + 10) = 0x12;
  lVar1 = luaS_newlstr(param_1,&DAT_00669ce5,4);
  *(byte *)(lVar1 + 9) = *(byte *)(lVar1 + 9) | 0x10;
  *(undefined1 *)(lVar1 + 10) = 0x13;
  lVar1 = luaS_newlstr(param_1,"until",5);
  *(byte *)(lVar1 + 9) = *(byte *)(lVar1 + 9) | 0x10;
  *(undefined1 *)(lVar1 + 10) = 0x14;
  lVar1 = luaS_newlstr(param_1,"while",5);
  *(byte *)(lVar1 + 9) = *(byte *)(lVar1 + 9) | 0x10;
  *(undefined1 *)(lVar1 + 10) = 0x15;
  return;
}



// ======== luaX_checklimit @ 0064ca60 ========

void luaX_checklimit(long param_1,int param_2,int param_3,undefined8 param_4)

{
  undefined8 uVar1;
  
  if (param_3 < param_2) {
    uVar1 = luaO_pushfstring(*(undefined8 *)(param_1 + 0x38),"too many %s (limit=%d)",param_4,
                             param_3);
    luaX_syntaxerror(param_1,uVar1);
    return;
  }
  return;
}



// ======== luaX_syntaxerror @ 0064ca90 ========

void luaX_syntaxerror(long param_1,undefined8 param_2)

{
  int iVar1;
  undefined4 uVar2;
  undefined8 uVar3;
  long lVar4;
  undefined1 auStack_78 [80];
  
  iVar1 = *(int *)(param_1 + 0x10);
  if (iVar1 - 0x11dU < 2) {
    lVar4 = **(long **)(param_1 + 0x48);
  }
  else if (iVar1 == 0x116) {
    lVar4 = *(long *)(param_1 + 0x18) + 0x18;
  }
  else if (iVar1 < 0x101) {
    lVar4 = luaO_pushfstring(*(undefined8 *)(param_1 + 0x38),"%c");
  }
  else {
    lVar4 = *(long *)(token2string + (ulong)(iVar1 - 0x101) * 8);
  }
  uVar2 = *(undefined4 *)(param_1 + 4);
  uVar3 = *(undefined8 *)(param_1 + 0x38);
  luaO_chunkid(auStack_78,*(long *)(param_1 + 0x50) + 0x18,0x50);
  luaO_pushfstring(uVar3,"%s:%d: %s near `%s\'",auStack_78,uVar2,param_2,lVar4);
  luaD_throw(uVar3,3);
  return;
}



// ======== luaX_errorline @ 0064cb50 ========

void luaX_errorline(long param_1,undefined8 param_2,undefined8 param_3,undefined4 param_4)

{
  undefined8 uVar1;
  undefined1 auStack_78 [80];
  
  uVar1 = *(undefined8 *)(param_1 + 0x38);
  luaO_chunkid(auStack_78,*(long *)(param_1 + 0x50) + 0x18,0x50);
  luaO_pushfstring(uVar1,"%s:%d: %s near `%s\'",auStack_78,param_4,param_2,param_3);
  luaD_throw(uVar1,3);
  return;
}



// ======== luaX_token2str @ 0064cbc0 ========

undefined8 luaX_token2str(long param_1,int param_2)

{
  undefined8 uVar1;
  
  if (param_2 < 0x101) {
    uVar1 = luaO_pushfstring(*(undefined8 *)(param_1 + 0x38),"%c");
    return uVar1;
  }
  return *(undefined8 *)(token2string + (ulong)(param_2 - 0x101) * 8);
}



// ======== luaX_error @ 0064cbf0 ========

void luaX_error(undefined4 param_1,undefined8 param_2,long param_3,undefined8 param_4,
               undefined8 param_5)

{
  undefined1 auStack_78 [80];
  
  luaO_chunkid(auStack_78,param_3 + 0x18,0x50);
  luaO_pushfstring(param_2,"%s:%d: %s near `%s\'",auStack_78,param_1,param_4,param_5);
  luaD_throw(param_2,3);
  return;
}



// ======== luaX_setinput @ 0064cc50 ========

void luaX_setinput(undefined8 param_1,uint *param_2,long *param_3,undefined8 param_4)

{
  long lVar1;
  long *plVar2;
  byte *pbVar3;
  uint uVar4;
  
  *(undefined8 *)(param_2 + 0xe) = param_1;
  param_2[8] = 0x11f;
  *(long **)(param_2 + 0x10) = param_3;
  param_2[0xc] = 0;
  param_2[0xd] = 0;
  param_2[1] = 1;
  param_2[2] = 1;
  *(undefined8 *)(param_2 + 0x14) = param_4;
  lVar1 = *param_3;
  *param_3 = *param_3 + -1;
  if (lVar1 == 0) {
    uVar4 = luaZ_fill(param_3);
    *param_2 = uVar4;
  }
  else {
    pbVar3 = (byte *)param_3[1];
    param_3[1] = (long)(pbVar3 + 1);
    uVar4 = (uint)*pbVar3;
    *param_2 = uVar4;
  }
  if (uVar4 == 0x23) {
    do {
      plVar2 = *(long **)(param_2 + 0x10);
      lVar1 = *plVar2;
      *plVar2 = *plVar2 + -1;
      if (lVar1 == 0) {
        uVar4 = luaZ_fill();
        *param_2 = uVar4;
      }
      else {
        pbVar3 = (byte *)plVar2[1];
        plVar2[1] = (long)(pbVar3 + 1);
        uVar4 = (uint)*pbVar3;
        *param_2 = uVar4;
      }
    } while ((uVar4 != 10) && (uVar4 != 0xffffffff));
  }
  return;
}



// ======== luaX_lex @ 0064ccf0 ========

/* WARNING: Type propagation algorithm not settling */

ulong luaX_lex(uint *param_1,long *param_2)

{
  uint *puVar1;
  long lVar2;
  byte bVar3;
  ushort uVar4;
  byte *pbVar5;
  undefined8 uVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  ulong uVar10;
  ushort **ppuVar11;
  undefined8 uVar12;
  int iVar13;
  undefined8 *puVar14;
  long *plVar15;
  long lVar16;
  long local_a0;
  undefined1 local_88 [88];
  
  puVar1 = param_1 + 0x10;
LAB_0064cd20:
  uVar7 = *param_1;
  uVar10 = 0x11f;
  switch(uVar7) {
  case 10:
    plVar15 = *(long **)puVar1;
    lVar16 = *plVar15;
    *plVar15 = *plVar15 + -1;
    if (lVar16 == 0) {
      uVar7 = luaZ_fill();
    }
    else {
      pbVar5 = (byte *)plVar15[1];
      plVar15[1] = (long)(pbVar5 + 1);
      uVar7 = (uint)*pbVar5;
    }
    *param_1 = uVar7;
    uVar7 = param_1[1];
    param_1[1] = uVar7 + 1;
    if (0x7ffffffc < (int)uVar7) {
      luaO_pushfstring(*(undefined8 *)(param_1 + 0xe),"too many %s (limit=%d)","lines in a chunk",
                       0x7ffffffd);
      luaX_syntaxerror(param_1);
    }
    goto LAB_0064cd20;
  case 0x22:
  case 0x27:
    puVar14 = *(undefined8 **)(param_1 + 0x12);
    uVar9 = uVar7;
    if ((ulong)puVar14[1] < 5) {
      luaZ_openspace(*(undefined8 *)(param_1 + 0xe),puVar14,0x20);
      puVar14 = *(undefined8 **)(param_1 + 0x12);
      uVar9 = *param_1;
    }
    *(char *)*puVar14 = (char)uVar9;
    plVar15 = *(long **)puVar1;
    lVar16 = *plVar15;
    *plVar15 = *plVar15 + -1;
    if (lVar16 == 0) {
      uVar9 = luaZ_fill();
    }
    else {
      pbVar5 = (byte *)plVar15[1];
      plVar15[1] = (long)(pbVar5 + 1);
      uVar9 = (uint)*pbVar5;
    }
    *param_1 = uVar9;
    local_a0 = 1;
    if (uVar9 == uVar7) goto LAB_0064d02d;
    local_a0 = 1;
    break;
  case 0x2d:
    plVar15 = *(long **)puVar1;
    lVar16 = *plVar15;
    *plVar15 = *plVar15 + -1;
    if (lVar16 == 0) {
      uVar7 = luaZ_fill();
      *param_1 = uVar7;
    }
    else {
      pbVar5 = (byte *)plVar15[1];
      plVar15[1] = (long)(pbVar5 + 1);
      uVar7 = (uint)*pbVar5;
      *param_1 = uVar7;
    }
    if (uVar7 != 0x2d) {
      return 0x2d;
    }
    plVar15 = *(long **)puVar1;
    lVar16 = *plVar15;
    *plVar15 = *plVar15 + -1;
    if (lVar16 == 0) {
      uVar7 = luaZ_fill();
      *param_1 = uVar7;
    }
    else {
      pbVar5 = (byte *)plVar15[1];
      plVar15[1] = (long)(pbVar5 + 1);
      uVar7 = (uint)*pbVar5;
      *param_1 = uVar7;
    }
    if (uVar7 == 0x5b) {
      plVar15 = *(long **)puVar1;
      lVar16 = *plVar15;
      *plVar15 = *plVar15 + -1;
      if (lVar16 == 0) {
        uVar7 = luaZ_fill();
        *param_1 = uVar7;
      }
      else {
        pbVar5 = (byte *)plVar15[1];
        plVar15[1] = (long)(pbVar5 + 1);
        uVar7 = (uint)*pbVar5;
        *param_1 = uVar7;
      }
      if (uVar7 != 0x5b) goto LAB_0064cea8;
      read_long_string(param_1,0);
    }
    else {
LAB_0064cea8:
      while ((uVar7 != 0xffffffff && (uVar7 != 10))) {
        plVar15 = *(long **)puVar1;
        lVar16 = *plVar15;
        *plVar15 = *plVar15 + -1;
        if (lVar16 == 0) {
          uVar7 = luaZ_fill();
        }
        else {
          pbVar5 = (byte *)plVar15[1];
          plVar15[1] = (long)(pbVar5 + 1);
          uVar7 = (uint)*pbVar5;
        }
        *param_1 = uVar7;
      }
    }
    goto LAB_0064cd20;
  case 0x2e:
    plVar15 = *(long **)puVar1;
    lVar16 = *plVar15;
    *plVar15 = *plVar15 + -1;
    if (lVar16 == 0) {
      uVar7 = luaZ_fill();
      *param_1 = uVar7;
    }
    else {
      pbVar5 = (byte *)plVar15[1];
      plVar15[1] = (long)(pbVar5 + 1);
      uVar7 = (uint)*pbVar5;
      *param_1 = uVar7;
    }
    if (uVar7 == 0x2e) {
      plVar15 = *(long **)puVar1;
      lVar16 = *plVar15;
      *plVar15 = *plVar15 + -1;
      if (lVar16 == 0) {
        uVar7 = luaZ_fill();
      }
      else {
        pbVar5 = (byte *)plVar15[1];
        plVar15[1] = (long)(pbVar5 + 1);
        uVar7 = (uint)*pbVar5;
      }
      *param_1 = uVar7;
      if (uVar7 != 0x2e) {
        return 0x117;
      }
      plVar15 = *(long **)puVar1;
      lVar16 = *plVar15;
      *plVar15 = *plVar15 + -1;
      if (lVar16 == 0) {
        uVar7 = luaZ_fill();
      }
      else {
        pbVar5 = (byte *)plVar15[1];
        plVar15[1] = (long)(pbVar5 + 1);
        uVar7 = (uint)*pbVar5;
      }
      *param_1 = uVar7;
      return 0x118;
    }
    ppuVar11 = __ctype_b_loc();
    if ((*(byte *)((long)*ppuVar11 + (long)(int)uVar7 * 2 + 1) & 8) == 0) {
      return 0x2e;
    }
    uVar12 = 1;
    goto LAB_0064d6b2;
  case 0x3c:
    plVar15 = *(long **)puVar1;
    lVar16 = *plVar15;
    *plVar15 = *plVar15 + -1;
    if (lVar16 == 0) {
      uVar7 = luaZ_fill();
      *param_1 = uVar7;
    }
    else {
      pbVar5 = (byte *)plVar15[1];
      plVar15[1] = (long)(pbVar5 + 1);
      uVar7 = (uint)*pbVar5;
      *param_1 = uVar7;
    }
    if (uVar7 != 0x3d) {
      return 0x3c;
    }
    plVar15 = *(long **)puVar1;
    lVar16 = *plVar15;
    *plVar15 = *plVar15 + -1;
    if (lVar16 == 0) {
      uVar7 = luaZ_fill();
    }
    else {
      pbVar5 = (byte *)plVar15[1];
      plVar15[1] = (long)(pbVar5 + 1);
      uVar7 = (uint)*pbVar5;
    }
    *param_1 = uVar7;
    return 0x11b;
  case 0x3d:
    plVar15 = *(long **)puVar1;
    lVar16 = *plVar15;
    *plVar15 = *plVar15 + -1;
    if (lVar16 == 0) {
      uVar7 = luaZ_fill();
      *param_1 = uVar7;
    }
    else {
      pbVar5 = (byte *)plVar15[1];
      plVar15[1] = (long)(pbVar5 + 1);
      uVar7 = (uint)*pbVar5;
      *param_1 = uVar7;
    }
    if (uVar7 != 0x3d) {
      return 0x3d;
    }
    plVar15 = *(long **)puVar1;
    lVar16 = *plVar15;
    *plVar15 = *plVar15 + -1;
    if (lVar16 == 0) {
      uVar7 = luaZ_fill();
    }
    else {
      pbVar5 = (byte *)plVar15[1];
      plVar15[1] = (long)(pbVar5 + 1);
      uVar7 = (uint)*pbVar5;
    }
    *param_1 = uVar7;
    return 0x119;
  case 0x3e:
    plVar15 = *(long **)puVar1;
    lVar16 = *plVar15;
    *plVar15 = *plVar15 + -1;
    if (lVar16 == 0) {
      uVar7 = luaZ_fill();
      *param_1 = uVar7;
    }
    else {
      pbVar5 = (byte *)plVar15[1];
      plVar15[1] = (long)(pbVar5 + 1);
      uVar7 = (uint)*pbVar5;
      *param_1 = uVar7;
    }
    if (uVar7 != 0x3d) {
      return 0x3e;
    }
    plVar15 = *(long **)puVar1;
    lVar16 = *plVar15;
    *plVar15 = *plVar15 + -1;
    if (lVar16 == 0) {
      uVar7 = luaZ_fill();
    }
    else {
      pbVar5 = (byte *)plVar15[1];
      plVar15[1] = (long)(pbVar5 + 1);
      uVar7 = (uint)*pbVar5;
    }
    *param_1 = uVar7;
    return 0x11a;
  case 0x5b:
    plVar15 = *(long **)puVar1;
    lVar16 = *plVar15;
    *plVar15 = *plVar15 + -1;
    if (lVar16 == 0) {
      uVar7 = luaZ_fill();
      *param_1 = uVar7;
    }
    else {
      pbVar5 = (byte *)plVar15[1];
      plVar15[1] = (long)(pbVar5 + 1);
      uVar7 = (uint)*pbVar5;
      *param_1 = uVar7;
    }
    if (uVar7 != 0x5b) {
      return 0x5b;
    }
    read_long_string(param_1,param_2);
    return 0x11e;
  case 0xffffffff:
    goto switchD_0064cd31_caseD_ffffffff;
  default:
    if (uVar7 == 0x7e) {
      plVar15 = *(long **)puVar1;
      lVar16 = *plVar15;
      *plVar15 = *plVar15 + -1;
      if (lVar16 == 0) {
        uVar7 = luaZ_fill();
      }
      else {
        pbVar5 = (byte *)plVar15[1];
        plVar15[1] = (long)(pbVar5 + 1);
        uVar7 = (uint)*pbVar5;
      }
      *param_1 = uVar7;
      if (uVar7 != 0x3d) {
        return 0x7e;
      }
      plVar15 = *(long **)puVar1;
      lVar16 = *plVar15;
      *plVar15 = *plVar15 + -1;
      if (lVar16 == 0) {
        uVar7 = luaZ_fill();
      }
      else {
        pbVar5 = (byte *)plVar15[1];
        plVar15[1] = (long)(pbVar5 + 1);
        uVar7 = (uint)*pbVar5;
      }
      *param_1 = uVar7;
      return 0x11c;
    }
  case 0:
  case 1:
  case 2:
  case 3:
  case 4:
  case 5:
  case 6:
  case 7:
  case 8:
  case 9:
  case 0xb:
  case 0xc:
  case 0xd:
  case 0xe:
  case 0xf:
  case 0x10:
  case 0x11:
  case 0x12:
  case 0x13:
  case 0x14:
  case 0x15:
  case 0x16:
  case 0x17:
  case 0x18:
  case 0x19:
  case 0x1a:
  case 0x1b:
  case 0x1c:
  case 0x1d:
  case 0x1e:
  case 0x1f:
  case 0x20:
  case 0x21:
  case 0x23:
  case 0x24:
  case 0x25:
  case 0x26:
  case 0x28:
  case 0x29:
  case 0x2a:
  case 0x2b:
  case 0x2c:
  case 0x2f:
  case 0x30:
  case 0x31:
  case 0x32:
  case 0x33:
  case 0x34:
  case 0x35:
  case 0x36:
  case 0x37:
  case 0x38:
  case 0x39:
  case 0x3a:
  case 0x3b:
  case 0x3f:
  case 0x40:
  case 0x41:
  case 0x42:
  case 0x43:
  case 0x44:
  case 0x45:
  case 0x46:
  case 0x47:
  case 0x48:
  case 0x49:
  case 0x4a:
  case 0x4b:
  case 0x4c:
  case 0x4d:
  case 0x4e:
  case 0x4f:
  case 0x50:
  case 0x51:
  case 0x52:
  case 0x53:
  case 0x54:
  case 0x55:
  case 0x56:
  case 0x57:
  case 0x58:
  case 0x59:
  case 0x5a:
    ppuVar11 = __ctype_b_loc();
    uVar4 = (*ppuVar11)[(int)uVar7];
    if ((uVar4 & 0x2000) == 0) {
      if ((uVar4 & 0x800) == 0) {
        if (((uVar4 & 0x400) == 0) && (uVar7 != 0x5f)) {
          if ((uVar4 & 2) != 0) {
            uVar12 = luaO_pushfstring(*(undefined8 *)(param_1 + 0xe),"char(%d)",uVar7);
            luaX_error(param_1[1],*(undefined8 *)(param_1 + 0xe),*(undefined8 *)(param_1 + 0x14),
                       "invalid control char",uVar12);
          }
          plVar15 = *(long **)puVar1;
          lVar16 = *plVar15;
          *plVar15 = *plVar15 + -1;
          if (lVar16 == 0) {
            uVar9 = luaZ_fill();
          }
          else {
            pbVar5 = (byte *)plVar15[1];
            plVar15[1] = (long)(pbVar5 + 1);
            uVar9 = (uint)*pbVar5;
          }
          *param_1 = uVar9;
          return (ulong)uVar7;
        }
        if (*(ulong *)(*(long *)(param_1 + 0x12) + 8) < 5) {
          luaZ_openspace(*(undefined8 *)(param_1 + 0xe),*(long *)(param_1 + 0x12),0x20);
        }
        lVar16 = 0;
        do {
          plVar15 = *(long **)(param_1 + 0x12);
          if ((ulong)plVar15[1] < lVar16 + 5U) {
            luaZ_openspace(*(undefined8 *)(param_1 + 0xe),plVar15,lVar16 + 0x20);
            plVar15 = *(long **)(param_1 + 0x12);
          }
          *(char *)(*plVar15 + lVar16) = (char)*param_1;
          plVar15 = *(long **)(param_1 + 0x10);
          lVar2 = *plVar15;
          *plVar15 = *plVar15 + -1;
          if (lVar2 == 0) {
            uVar7 = luaZ_fill();
          }
          else {
            pbVar5 = (byte *)plVar15[1];
            plVar15[1] = (long)(pbVar5 + 1);
            uVar7 = (uint)*pbVar5;
          }
          *param_1 = uVar7;
          lVar16 = lVar16 + 1;
        } while ((((*ppuVar11)[(int)uVar7] & 8) != 0) || (uVar7 == 0x5f));
        *(undefined1 *)(**(long **)(param_1 + 0x12) + lVar16) = 0;
        lVar16 = luaS_newlstr(*(undefined8 *)(param_1 + 0xe),**(undefined8 **)(param_1 + 0x12),
                              lVar16);
        if (*(byte *)(lVar16 + 10) != 0) {
          return (ulong)(*(byte *)(lVar16 + 10) | 0x100);
        }
        *param_2 = lVar16;
        return 0x116;
      }
      uVar12 = 0;
LAB_0064d6b2:
      read_numeral(param_1,uVar12,param_2);
      return 0x11d;
    }
    plVar15 = *(long **)puVar1;
    lVar16 = *plVar15;
    *plVar15 = *plVar15 + -1;
    if (lVar16 == 0) {
      uVar7 = luaZ_fill();
      *param_1 = uVar7;
    }
    else {
      pbVar5 = (byte *)plVar15[1];
      plVar15[1] = (long)(pbVar5 + 1);
      *param_1 = (uint)*pbVar5;
    }
    goto LAB_0064cd20;
  }
LAB_0064d0e4:
  if (*(ulong *)(*(long *)(param_1 + 0x12) + 8) < local_a0 + 5U) {
    luaZ_openspace(*(undefined8 *)(param_1 + 0xe),*(long *)(param_1 + 0x12),local_a0 + 0x20);
    uVar9 = *param_1;
  }
  if (uVar9 == 0x5c) {
    plVar15 = *(long **)puVar1;
    lVar16 = *plVar15;
    *plVar15 = *plVar15 + -1;
    if (lVar16 == 0) {
      uVar9 = luaZ_fill();
      *param_1 = uVar9;
    }
    else {
      pbVar5 = (byte *)plVar15[1];
      plVar15[1] = (long)(pbVar5 + 1);
      bVar3 = *pbVar5;
      *param_1 = (uint)bVar3;
      uVar9 = (uint)bVar3;
    }
    if (uVar9 - 0x61 < 0x16) {
                    /* WARNING: Could not recover jumptable at 0x0064d174. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      uVar10 = (*(code *)(&PTR_DAT_0066c8c8)[uVar9 - 0x61])();
      return uVar10;
    }
    if (uVar9 != 0xffffffff) {
      if (uVar9 == 10) {
        *(undefined1 *)(**(long **)(param_1 + 0x12) + local_a0) = 10;
        plVar15 = *(long **)(param_1 + 0x10);
        lVar16 = *plVar15;
        *plVar15 = *plVar15 + -1;
        if (lVar16 == 0) {
          uVar9 = luaZ_fill();
        }
        else {
          pbVar5 = (byte *)plVar15[1];
          plVar15[1] = (long)(pbVar5 + 1);
          uVar9 = (uint)*pbVar5;
        }
        local_a0 = local_a0 + 1;
        *param_1 = uVar9;
        uVar9 = param_1[1];
        param_1[1] = uVar9 + 1;
        if (0x7ffffffc < (int)uVar9) {
          uVar12 = luaO_pushfstring(*(undefined8 *)(param_1 + 0xe),"too many %s (limit=%d)",
                                    "lines in a chunk",0x7ffffffd);
          luaX_syntaxerror(param_1,uVar12);
        }
      }
      else {
        ppuVar11 = __ctype_b_loc();
        if ((*(byte *)((long)*ppuVar11 + (long)(int)uVar9 * 2 + 1) & 8) == 0) {
          *(char *)(**(long **)(param_1 + 0x12) + local_a0) = (char)uVar9;
          plVar15 = *(long **)(param_1 + 0x10);
          lVar16 = *plVar15;
          *plVar15 = *plVar15 + -1;
          if (lVar16 == 0) {
            uVar9 = luaZ_fill();
          }
          else {
            pbVar5 = (byte *)plVar15[1];
            plVar15[1] = (long)(pbVar5 + 1);
            uVar9 = (uint)*pbVar5;
          }
          goto LAB_0064d322;
        }
        plVar15 = *(long **)puVar1;
        lVar16 = *plVar15;
        *plVar15 = *plVar15 + -1;
        if (lVar16 == 0) {
          uVar8 = luaZ_fill();
        }
        else {
          pbVar5 = (byte *)plVar15[1];
          plVar15[1] = (long)(pbVar5 + 1);
          uVar8 = (uint)*pbVar5;
        }
        iVar13 = uVar9 - 0x30;
        *param_1 = uVar8;
        if ((*(byte *)((long)*ppuVar11 + (long)(int)uVar8 * 2 + 1) & 8) != 0) {
          plVar15 = *(long **)puVar1;
          lVar16 = *plVar15;
          *plVar15 = *plVar15 + -1;
          if (lVar16 == 0) {
            uVar9 = luaZ_fill();
          }
          else {
            pbVar5 = (byte *)plVar15[1];
            plVar15[1] = (long)(pbVar5 + 1);
            uVar9 = (uint)*pbVar5;
          }
          iVar13 = uVar8 + iVar13 * 10 + -0x30;
          *param_1 = uVar9;
          if ((*(byte *)((long)*ppuVar11 + (long)(int)uVar9 * 2 + 1) & 8) != 0) {
            plVar15 = *(long **)puVar1;
            lVar16 = *plVar15;
            *plVar15 = *plVar15 + -1;
            if (lVar16 == 0) {
              uVar8 = luaZ_fill();
            }
            else {
              pbVar5 = (byte *)plVar15[1];
              plVar15[1] = (long)(pbVar5 + 1);
              uVar8 = (uint)*pbVar5;
            }
            iVar13 = (uVar9 - 0x30) + iVar13 * 10;
            *param_1 = uVar8;
          }
        }
        if (0xff < iVar13) {
          *(undefined1 *)(**(long **)(param_1 + 0x12) + local_a0) = 0;
          local_a0 = local_a0 + 1;
          uVar12 = **(undefined8 **)(param_1 + 0x12);
          uVar9 = param_1[1];
          uVar6 = *(undefined8 *)(param_1 + 0xe);
          luaO_chunkid(local_88,*(long *)(param_1 + 0x14) + 0x18,0x50);
          luaO_pushfstring(uVar6,"%s:%d: %s near `%s\'",local_88,uVar9,"escape sequence too large",
                           uVar12);
          luaD_throw(uVar6,3);
        }
        *(char *)(**(long **)(param_1 + 0x12) + local_a0) = (char)iVar13;
        local_a0 = local_a0 + 1;
      }
    }
  }
  else {
    if (uVar9 == 10) {
      *(undefined1 *)(**(long **)(param_1 + 0x12) + local_a0) = 0;
      uVar6 = **(undefined8 **)(param_1 + 0x12);
      uVar9 = param_1[1];
      uVar12 = *(undefined8 *)(param_1 + 0xe);
      luaO_chunkid(local_88,*(long *)(param_1 + 0x14) + 0x18,0x50);
      luaO_pushfstring(uVar12,"%s:%d: %s near `%s\'",local_88,uVar9,"unfinished string",uVar6);
    }
    else {
      if (uVar9 != 0xffffffff) {
        *(char *)(**(long **)(param_1 + 0x12) + local_a0) = (char)uVar9;
        plVar15 = *(long **)(param_1 + 0x10);
        lVar16 = *plVar15;
        *plVar15 = *plVar15 + -1;
        if (lVar16 == 0) {
          uVar9 = luaZ_fill();
        }
        else {
          pbVar5 = (byte *)plVar15[1];
          plVar15[1] = (long)(pbVar5 + 1);
          uVar9 = (uint)*pbVar5;
        }
LAB_0064d322:
        local_a0 = local_a0 + 1;
        *param_1 = uVar9;
        goto LAB_0064d0d8;
      }
      *(undefined1 *)(**(long **)(param_1 + 0x12) + local_a0) = 0;
      uVar9 = param_1[1];
      uVar12 = *(undefined8 *)(param_1 + 0xe);
      luaO_chunkid(local_88,*(long *)(param_1 + 0x14) + 0x18,0x50);
      luaO_pushfstring(uVar12,"%s:%d: %s near `%s\'",local_88,uVar9,"unfinished string","<eof>");
    }
    local_a0 = local_a0 + 1;
    luaD_throw(uVar12,3);
  }
LAB_0064d0d8:
  uVar9 = *param_1;
  if (uVar9 == uVar7) {
LAB_0064d02d:
    *(char *)(**(long **)(param_1 + 0x12) + local_a0) = (char)uVar7;
    plVar15 = *(long **)(param_1 + 0x10);
    lVar16 = *plVar15;
    *plVar15 = *plVar15 + -1;
    if (lVar16 == 0) {
      uVar7 = luaZ_fill();
    }
    else {
      pbVar5 = (byte *)plVar15[1];
      plVar15[1] = (long)(pbVar5 + 1);
      uVar7 = (uint)*pbVar5;
    }
    *param_1 = uVar7;
    *(undefined1 *)(local_a0 + 1 + **(long **)(param_1 + 0x12)) = 0;
    lVar16 = luaS_newlstr(*(undefined8 *)(param_1 + 0xe),**(long **)(param_1 + 0x12) + 1,
                          local_a0 + -1);
    *param_2 = lVar16;
    uVar10 = 0x11e;
switchD_0064cd31_caseD_ffffffff:
    return uVar10;
  }
  goto LAB_0064d0e4;
}



// ======== read_long_string @ 0064d8b0 ========

void read_long_string(uint *param_1,undefined8 *param_2)

{
  long lVar1;
  byte *pbVar2;
  uint uVar3;
  undefined8 uVar4;
  char *pcVar5;
  undefined8 *puVar6;
  long *plVar7;
  long lVar8;
  int local_94;
  undefined1 local_88 [88];
  
  puVar6 = *(undefined8 **)(param_1 + 0x12);
  if ((ulong)puVar6[1] < 5) {
    luaZ_openspace(*(undefined8 *)(param_1 + 0xe),puVar6,0x20);
    puVar6 = *(undefined8 **)(param_1 + 0x12);
  }
  *(undefined1 *)*puVar6 = 0x5b;
  *(char *)(**(long **)(param_1 + 0x12) + 1) = (char)*param_1;
  plVar7 = *(long **)(param_1 + 0x10);
  lVar1 = *plVar7;
  *plVar7 = *plVar7 + -1;
  if (lVar1 == 0) {
    uVar3 = luaZ_fill();
    *param_1 = uVar3;
  }
  else {
    pbVar2 = (byte *)plVar7[1];
    plVar7[1] = (long)(pbVar2 + 1);
    uVar3 = (uint)*pbVar2;
    *param_1 = uVar3;
  }
  if (uVar3 == 10) {
    plVar7 = *(long **)(param_1 + 0x10);
    lVar1 = *plVar7;
    *plVar7 = *plVar7 + -1;
    if (lVar1 == 0) {
      uVar3 = luaZ_fill();
    }
    else {
      pbVar2 = (byte *)plVar7[1];
      plVar7[1] = (long)(pbVar2 + 1);
      uVar3 = (uint)*pbVar2;
    }
    *param_1 = uVar3;
    uVar3 = param_1[1];
    param_1[1] = uVar3 + 1;
    if (0x7ffffffc < (int)uVar3) {
      uVar4 = luaO_pushfstring(*(undefined8 *)(param_1 + 0xe),"too many %s (limit=%d)",
                               "lines in a chunk",0x7ffffffd);
      luaX_syntaxerror(param_1,uVar4);
    }
  }
  pcVar5 = "unfinished long string";
  if (param_2 == (undefined8 *)0x0) {
    pcVar5 = "unfinished long comment";
  }
  local_94 = 0;
  lVar1 = 2;
LAB_0064d9e7:
  while (lVar8 = lVar1, lVar8 + 5U <= *(ulong *)(*(long *)(param_1 + 0x12) + 8)) {
    uVar3 = *param_1;
    if (9 < (int)uVar3) goto LAB_0064d9fc;
LAB_0064da44:
    if (uVar3 != 0xffffffff) goto LAB_0064dab8;
    *(undefined1 *)(**(long **)(param_1 + 0x12) + lVar8) = 0;
    uVar3 = param_1[1];
    uVar4 = *(undefined8 *)(param_1 + 0xe);
    luaO_chunkid(local_88,*(long *)(param_1 + 0x14) + 0x18,0x50);
    luaO_pushfstring(uVar4,"%s:%d: %s near `%s\'",local_88,uVar3,pcVar5,"<eof>");
    luaD_throw(uVar4,3);
    lVar1 = lVar8 + 1;
  }
  luaZ_openspace(*(undefined8 *)(param_1 + 0xe),*(long *)(param_1 + 0x12),lVar8 + 0x20);
  uVar3 = *param_1;
  if ((int)uVar3 < 10) goto LAB_0064da44;
LAB_0064d9fc:
  if (uVar3 == 10) {
    *(undefined1 *)(**(long **)(param_1 + 0x12) + lVar8) = 10;
    plVar7 = *(long **)(param_1 + 0x10);
    lVar1 = *plVar7;
    *plVar7 = *plVar7 + -1;
    if (lVar1 == 0) {
      uVar3 = luaZ_fill();
    }
    else {
      pbVar2 = (byte *)plVar7[1];
      plVar7[1] = (long)(pbVar2 + 1);
      uVar3 = (uint)*pbVar2;
    }
    *param_1 = uVar3;
    uVar3 = param_1[1];
    param_1[1] = uVar3 + 1;
    if (0x7ffffffc < (int)uVar3) {
      uVar4 = luaO_pushfstring(*(undefined8 *)(param_1 + 0xe),"too many %s (limit=%d)",
                               "lines in a chunk",0x7ffffffd);
      luaX_syntaxerror(param_1,uVar4);
    }
    lVar1 = lVar8 + 1;
    if (param_2 == (undefined8 *)0x0) {
      lVar1 = 0;
    }
    goto LAB_0064d9e7;
  }
  if (uVar3 == 0x5b) {
    *(undefined1 *)(**(long **)(param_1 + 0x12) + lVar8) = 0x5b;
    plVar7 = *(long **)(param_1 + 0x10);
    lVar1 = *plVar7;
    *plVar7 = *plVar7 + -1;
    if (lVar1 == 0) {
      uVar3 = luaZ_fill();
      *param_1 = uVar3;
    }
    else {
      pbVar2 = (byte *)plVar7[1];
      plVar7[1] = (long)(pbVar2 + 1);
      uVar3 = (uint)*pbVar2;
      *param_1 = uVar3;
    }
    lVar1 = lVar8 + 1;
    if (uVar3 != 0x5b) goto LAB_0064d9e7;
    local_94 = local_94 + 1;
    *(undefined1 *)(**(long **)(param_1 + 0x12) + lVar8 + 1) = 0x5b;
    plVar7 = *(long **)(param_1 + 0x10);
    lVar1 = *plVar7;
    *plVar7 = *plVar7 + -1;
    if (lVar1 != 0) goto LAB_0064dc28;
LAB_0064d9b0:
    uVar3 = luaZ_fill();
  }
  else {
    if (uVar3 != 0x5d) {
LAB_0064dab8:
      *(char *)(**(long **)(param_1 + 0x12) + lVar8) = (char)uVar3;
      plVar7 = *(long **)(param_1 + 0x10);
      lVar1 = *plVar7;
      *plVar7 = *plVar7 + -1;
      if (lVar1 == 0) {
        uVar3 = luaZ_fill();
      }
      else {
        pbVar2 = (byte *)plVar7[1];
        plVar7[1] = (long)(pbVar2 + 1);
        uVar3 = (uint)*pbVar2;
      }
      *param_1 = uVar3;
      lVar1 = lVar8 + 1;
      goto LAB_0064d9e7;
    }
    *(undefined1 *)(**(long **)(param_1 + 0x12) + lVar8) = 0x5d;
    plVar7 = *(long **)(param_1 + 0x10);
    lVar1 = *plVar7;
    *plVar7 = *plVar7 + -1;
    if (lVar1 == 0) {
      uVar3 = luaZ_fill();
      *param_1 = uVar3;
    }
    else {
      pbVar2 = (byte *)plVar7[1];
      plVar7[1] = (long)(pbVar2 + 1);
      uVar3 = (uint)*pbVar2;
      *param_1 = uVar3;
    }
    lVar1 = lVar8 + 1;
    if (uVar3 != 0x5d) goto LAB_0064d9e7;
    if (local_94 == 0) {
      *(undefined1 *)(**(long **)(param_1 + 0x12) + 1 + lVar8) = 0x5d;
      plVar7 = *(long **)(param_1 + 0x10);
      lVar1 = *plVar7;
      *plVar7 = *plVar7 + -1;
      if (lVar1 == 0) {
        uVar3 = luaZ_fill();
      }
      else {
        pbVar2 = (byte *)plVar7[1];
        plVar7[1] = (long)(pbVar2 + 1);
        uVar3 = (uint)*pbVar2;
      }
      *param_1 = uVar3;
      *(undefined1 *)(**(long **)(param_1 + 0x12) + 2 + lVar8) = 0;
      if (param_2 != (undefined8 *)0x0) {
        uVar4 = luaS_newlstr(*(undefined8 *)(param_1 + 0xe),**(long **)(param_1 + 0x12) + 2,
                             lVar8 + -2);
        *param_2 = uVar4;
      }
      return;
    }
    local_94 = local_94 + -1;
    *(undefined1 *)(**(long **)(param_1 + 0x12) + lVar8 + 1) = 0x5d;
    plVar7 = *(long **)(param_1 + 0x10);
    lVar1 = *plVar7;
    *plVar7 = *plVar7 + -1;
    if (lVar1 == 0) goto LAB_0064d9b0;
LAB_0064dc28:
    pbVar2 = (byte *)plVar7[1];
    plVar7[1] = (long)(pbVar2 + 1);
    uVar3 = (uint)*pbVar2;
  }
  *param_1 = uVar3;
  lVar1 = lVar8 + 2;
  goto LAB_0064d9e7;
}



// ======== read_numeral @ 0064dcb0 ========

void read_numeral(uint *param_1,int param_2,undefined8 param_3)

{
  long lVar1;
  byte bVar2;
  byte *pbVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined1 uVar6;
  uint uVar7;
  int iVar8;
  ushort **ppuVar9;
  ulong uVar10;
  ushort *puVar11;
  long *plVar12;
  ulong uVar13;
  undefined1 local_88 [88];
  
  if (*(ulong *)(*(long *)(param_1 + 0x12) + 8) < 5) {
    luaZ_openspace(*(undefined8 *)(param_1 + 0xe),*(long *)(param_1 + 0x12),0x20);
  }
  if (param_2 != 0) {
    *(undefined1 *)**(undefined8 **)(param_1 + 0x12) = 0x2e;
  }
  uVar13 = (ulong)(param_2 != 0);
  ppuVar9 = __ctype_b_loc();
  uVar10 = (ulong)(int)*param_1;
  bVar2 = *(byte *)((long)*ppuVar9 + uVar10 * 2 + 1);
  while ((bVar2 & 8) != 0) {
    uVar6 = (undefined1)uVar10;
    plVar12 = *(long **)(param_1 + 0x12);
    if ((ulong)plVar12[1] < uVar13 + 5) {
      luaZ_openspace(*(undefined8 *)(param_1 + 0xe),plVar12,uVar13 + 0x20);
      uVar6 = (undefined1)*param_1;
      plVar12 = *(long **)(param_1 + 0x12);
    }
    *(undefined1 *)(*plVar12 + uVar13) = uVar6;
    plVar12 = *(long **)(param_1 + 0x10);
    lVar1 = *plVar12;
    *plVar12 = *plVar12 + -1;
    if (lVar1 == 0) {
      uVar10 = luaZ_fill();
    }
    else {
      pbVar3 = (byte *)plVar12[1];
      plVar12[1] = (long)(pbVar3 + 1);
      uVar10 = (ulong)*pbVar3;
    }
    *param_1 = (uint)uVar10;
    uVar13 = uVar13 + 1;
    bVar2 = *(byte *)((long)*ppuVar9 + (long)(int)(uint)uVar10 * 2 + 1);
  }
  if ((int)uVar10 == 0x2e) {
    *(undefined1 *)(**(long **)(param_1 + 0x12) + uVar13) = 0x2e;
    plVar12 = *(long **)(param_1 + 0x10);
    lVar1 = *plVar12;
    *plVar12 = *plVar12 + -1;
    if (lVar1 == 0) {
      uVar10 = luaZ_fill();
      uVar7 = (uint)uVar10;
      *param_1 = uVar7;
    }
    else {
      pbVar3 = (byte *)plVar12[1];
      plVar12[1] = (long)(pbVar3 + 1);
      bVar2 = *pbVar3;
      uVar10 = (ulong)bVar2;
      uVar7 = (uint)bVar2;
      *param_1 = uVar7;
    }
    if (uVar7 == 0x2e) {
      *(undefined1 *)(**(long **)(param_1 + 0x12) + 1 + uVar13) = 0x2e;
      plVar12 = *(long **)(param_1 + 0x10);
      lVar1 = *plVar12;
      *plVar12 = *plVar12 + -1;
      if (lVar1 == 0) {
        uVar7 = luaZ_fill();
      }
      else {
        pbVar3 = (byte *)plVar12[1];
        plVar12[1] = (long)(pbVar3 + 1);
        uVar7 = (uint)*pbVar3;
      }
      *param_1 = uVar7;
      *(undefined1 *)(**(long **)(param_1 + 0x12) + 2 + uVar13) = 0;
      uVar13 = uVar13 + 3;
      uVar4 = **(undefined8 **)(param_1 + 0x12);
      uVar7 = param_1[1];
      uVar5 = *(undefined8 *)(param_1 + 0xe);
      luaO_chunkid(local_88,*(long *)(param_1 + 0x14) + 0x18,0x50);
      luaO_pushfstring(uVar5,"%s:%d: %s near `%s\'",local_88,uVar7,
                       "ambiguous syntax (decimal point x string concatenation)",uVar4);
      luaD_throw(uVar5,3);
      uVar10 = (ulong)*param_1;
    }
    else {
      uVar13 = uVar13 + 1;
    }
  }
  uVar7 = (uint)uVar10;
  puVar11 = *ppuVar9;
  while ((*(byte *)((long)puVar11 + (long)(int)uVar7 * 2 + 1) & 8) != 0) {
    plVar12 = *(long **)(param_1 + 0x12);
    if ((ulong)plVar12[1] < uVar13 + 5) {
      luaZ_openspace(*(undefined8 *)(param_1 + 0xe),plVar12,uVar13 + 0x20);
      uVar10 = (ulong)*param_1;
      plVar12 = *(long **)(param_1 + 0x12);
    }
    *(char *)(*plVar12 + uVar13) = (char)uVar10;
    plVar12 = *(long **)(param_1 + 0x10);
    lVar1 = *plVar12;
    *plVar12 = *plVar12 + -1;
    if (lVar1 == 0) {
      uVar10 = luaZ_fill();
    }
    else {
      pbVar3 = (byte *)plVar12[1];
      plVar12[1] = (long)(pbVar3 + 1);
      uVar10 = (ulong)*pbVar3;
    }
    uVar7 = (uint)uVar10;
    *param_1 = uVar7;
    uVar13 = uVar13 + 1;
    puVar11 = *ppuVar9;
  }
  if (((uint)uVar10 | 0x20) != 0x65) goto LAB_0064dfbb;
  *(char *)(**(long **)(param_1 + 0x12) + uVar13) = (char)uVar10;
  plVar12 = *(long **)(param_1 + 0x10);
  lVar1 = *plVar12;
  *plVar12 = *plVar12 + -1;
  if (lVar1 == 0) {
    uVar7 = luaZ_fill();
    *param_1 = uVar7;
    if (uVar7 == 0x2d) goto LAB_0064df17;
LAB_0064df12:
    if (uVar7 == 0x2b) goto LAB_0064df17;
    uVar13 = uVar13 + 1;
  }
  else {
    pbVar3 = (byte *)plVar12[1];
    plVar12[1] = (long)(pbVar3 + 1);
    bVar2 = *pbVar3;
    uVar7 = (uint)bVar2;
    *param_1 = (uint)bVar2;
    if (bVar2 != 0x2d) goto LAB_0064df12;
LAB_0064df17:
    *(char *)(**(long **)(param_1 + 0x12) + 1 + uVar13) = (char)uVar7;
    plVar12 = *(long **)(param_1 + 0x10);
    lVar1 = *plVar12;
    *plVar12 = *plVar12 + -1;
    if (lVar1 == 0) {
      uVar7 = luaZ_fill();
    }
    else {
      pbVar3 = (byte *)plVar12[1];
      plVar12[1] = (long)(pbVar3 + 1);
      uVar7 = (uint)*pbVar3;
    }
    uVar13 = uVar13 + 2;
    *param_1 = uVar7;
  }
  puVar11 = *ppuVar9;
  while (uVar6 = (undefined1)uVar7, (*(byte *)((long)puVar11 + (long)(int)uVar7 * 2 + 1) & 8) != 0)
  {
    plVar12 = *(long **)(param_1 + 0x12);
    if ((ulong)plVar12[1] < uVar13 + 5) {
      luaZ_openspace(*(undefined8 *)(param_1 + 0xe),plVar12,uVar13 + 0x20);
      uVar6 = (undefined1)*param_1;
      plVar12 = *(long **)(param_1 + 0x12);
    }
    *(undefined1 *)(*plVar12 + uVar13) = uVar6;
    plVar12 = *(long **)(param_1 + 0x10);
    lVar1 = *plVar12;
    *plVar12 = *plVar12 + -1;
    if (lVar1 == 0) {
      uVar7 = luaZ_fill();
    }
    else {
      pbVar3 = (byte *)plVar12[1];
      plVar12[1] = (long)(pbVar3 + 1);
      uVar7 = (uint)*pbVar3;
    }
    *param_1 = uVar7;
    uVar13 = uVar13 + 1;
    puVar11 = *ppuVar9;
  }
LAB_0064dfbb:
  *(undefined1 *)(**(long **)(param_1 + 0x12) + uVar13) = 0;
  iVar8 = luaO_str2d(**(undefined8 **)(param_1 + 0x12),param_3);
  if (iVar8 == 0) {
    uVar4 = **(undefined8 **)(param_1 + 0x12);
    uVar7 = param_1[1];
    uVar5 = *(undefined8 *)(param_1 + 0xe);
    luaO_chunkid(local_88,*(long *)(param_1 + 0x14) + 0x18,0x50);
    luaO_pushfstring(uVar5,"%s:%d: %s near `%s\'",local_88,uVar7,"malformed number",uVar4);
    luaD_throw(uVar5,3);
  }
  return;
}



// ======== __libc_csu_init @ 0064e040 ========

int __libc_csu_init(void *param_1,void *param_2)

{
  int iVar1;
  long lVar2;
  
  _init(param_1);
  lVar2 = 0;
  do {
    iVar1 = (*(code *)(&__frame_dummy_init_array_entry)[lVar2])((ulong)param_1 & 0xffffffff,param_2)
    ;
    lVar2 = lVar2 + 1;
  } while (lVar2 != 0x17);
  return iVar1;
}



// ======== __libc_csu_fini @ 0064e0b0 ========

void __libc_csu_fini(void)

{
  return;
}



// ======== _fini @ 0064e0b8 ========

void _fini(void)

{
  return;
}



// ======== ftell @ 01211000 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

long ftell(FILE *__stream)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* ftell@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== SDL_DestroyWindow @ 01211008 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_DestroyWindow(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== SDL_malloc @ 01211010 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_malloc(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== __errno_location @ 01211018 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int * __errno_location(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* __errno_location@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== printf @ 01211020 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int printf(char *__format,...)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* printf@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== SDL_CreateWindow @ 01211028 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_CreateWindow(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== SDL_GameControllerFromInstanceID @ 01211030 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_GameControllerFromInstanceID(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== FAudio_CreateMasteringVoice @ 01211038 ========

/* WARNING: Control flow encountered bad instruction data */

void FAudio_CreateMasteringVoice(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== SDL_SetWindowPosition @ 01211040 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_SetWindowPosition(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== _M_hook @ 01211048 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::__detail::_List_node_base::_M_hook(_List_node_base *param_1)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZNSt8__detail15_List_node_base7_M_hookEPS0_@@GLIBCXX_3.4.15 */
  halt_baddata();
}



// ======== FAudioVoice_SetVolume @ 01211050 ========

/* WARNING: Control flow encountered bad instruction data */

void FAudioVoice_SetVolume(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== strcspn @ 01211058 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

size_t strcspn(char *__s,char *__reject)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* strcspn@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== SDL_Quit @ 01211060 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_Quit(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== atan2 @ 01211068 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

double atan2(double __y,double __x)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* atan2@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== compare @ 01211070 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::__cxx11::string::compare
               (ulong param_1,ulong param_2,string *param_3,ulong param_4,ulong param_5)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE7compareEmmRKS4_mm@@GLIBCXX_3.4.21
                        */
  halt_baddata();
}



// ======== __ctype_toupper_loc @ 01211078 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

__int32_t ** __ctype_toupper_loc(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* __ctype_toupper_loc@@GLIBC_2.3 */
  halt_baddata();
}



// ======== rewind @ 01211080 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void rewind(FILE *__stream)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* rewind@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== ungetc @ 01211088 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int ungetc(int __c,FILE *__stream)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* ungetc@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== SDL_Error @ 01211090 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_Error(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== _Rb_tree_insert_and_rebalance @ 01211098 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::_Rb_tree_insert_and_rebalance
               (bool param_1,_Rb_tree_node_base *param_2,_Rb_tree_node_base *param_3,
               _Rb_tree_node_base *param_4)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZSt29_Rb_tree_insert_and_rebalancebPSt18_Rb_tree_node_baseS0_RS_@@GLIBCXX_3.4
                        */
  halt_baddata();
}



// ======== FNA3D_GenVertexBuffer @ 012110a0 ========

/* WARNING: Control flow encountered bad instruction data */

void FNA3D_GenVertexBuffer(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== FAudioSourceVoice_SetFrequencyRatio @ 012110a8 ========

/* WARNING: Control flow encountered bad instruction data */

void FAudioSourceVoice_SetFrequencyRatio(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== ceil @ 012110b0 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

double ceil(double __x)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* ceil@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== __ctype_tolower_loc @ 012110b8 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

__int32_t ** __ctype_tolower_loc(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* __ctype_tolower_loc@@GLIBC_2.3 */
  halt_baddata();
}



// ======== sprintf @ 012110c0 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int sprintf(char *__s,char *__format,...)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* sprintf@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== _Hash_bytes @ 012110c8 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::_Hash_bytes(void *param_1,ulong param_2,ulong param_3)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZSt11_Hash_bytesPKvmm@@CXXABI_1.3.5 */
  halt_baddata();
}



// ======== SDL_RWclose @ 012110d0 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_RWclose(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== codecvt @ 012110d8 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::codecvt<wchar_t,char,__mbstate_t>::codecvt(ulong param_1)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZNSt7codecvtIwc11__mbstate_tEC2Em@@GLIBCXX_3.4 */
  halt_baddata();
}



// ======== sin @ 012110e0 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

double sin(double __x)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* sin@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== SDL_CreateRGBSurfaceWithFormat @ 012110e8 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_CreateRGBSurfaceWithFormat(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== strcoll @ 012110f0 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int strcoll(char *__s1,char *__s2)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* strcoll@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== FNA3D_SetViewport @ 012110f8 ========

/* WARNING: Control flow encountered bad instruction data */

void FNA3D_SetViewport(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== rand @ 01211100 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int rand(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* rand@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== terminate @ 01211108 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::terminate(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZSt9terminatev@@GLIBCXX_3.4 */
  halt_baddata();
}



// ======== compare @ 01211110 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::__cxx11::string::compare(char *param_1)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE7compareEPKc@@GLIBCXX_3.4.21
                        */
  halt_baddata();
}



// ======== FNA3D_AddDisposeEffect @ 01211118 ========

/* WARNING: Control flow encountered bad instruction data */

void FNA3D_AddDisposeEffect(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== getc @ 01211120 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int getc(FILE *__stream)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* getc@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== FNA3D_SetRenderTargets @ 01211128 ========

/* WARNING: Control flow encountered bad instruction data */

void FNA3D_SetRenderTargets(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== stb_vorbis_stream_length_in_samples @ 01211130 ========

/* WARNING: Control flow encountered bad instruction data */

void stb_vorbis_stream_length_in_samples(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== resize @ 01211138 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::__cxx11::string::resize(ulong param_1,char param_2)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE6resizeEmc@@GLIBCXX_3.4.21
                        */
  halt_baddata();
}



// ======== popen @ 01211140 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

FILE * popen(char *__command,char *__modes)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* popen@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== SDL_isdigit @ 01211148 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_isdigit(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== _M_append @ 01211150 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::__cxx11::string::_M_append(char *param_1,ulong param_2)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_appendEPKcm@@GLIBCXX_3.4.21
                        */
  halt_baddata();
}



// ======== SDL_sqrt @ 01211158 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_sqrt(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== FNA3D_Image_SavePNG @ 01211160 ========

/* WARNING: Control flow encountered bad instruction data */

void FNA3D_Image_SavePNG(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== strtoul @ 01211168 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

ulong strtoul(char *__nptr,char **__endptr,int __base)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* strtoul@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== log @ 01211170 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

double log(double __x)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* log@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== FT_Load_Glyph @ 01211178 ========

/* WARNING: Control flow encountered bad instruction data */

void FT_Load_Glyph(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== strerror @ 01211180 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

char * strerror(int __errnum)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* strerror@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== write @ 01211188 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

ssize_t write(int __fd,void *__buf,size_t __n)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* write@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== readdir64_r @ 01211190 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int readdir64_r(DIR *__dirp,dirent64 *__entry,dirent64 **__result)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* readdir64_r@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== SDL_strlen @ 01211198 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_strlen(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== __throw_bad_alloc @ 012111a0 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::__throw_bad_alloc(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZSt17__throw_bad_allocv@@GLIBCXX_3.4 */
  halt_baddata();
}



// ======== strchr @ 012111a8 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

char * strchr(char *__s,int __c)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* strchr@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== longjmp @ 012111b0 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void longjmp(__jmp_buf_tag *__env,int __val)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* longjmp@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== floorf @ 012111b8 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

float floorf(float __x)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* floorf@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== FAudioSourceVoice_Stop @ 012111c0 ========

/* WARNING: Control flow encountered bad instruction data */

void FAudioSourceVoice_Stop(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== runtime_error @ 012111c8 ========

/* WARNING: Control flow encountered bad instruction data */

void __thiscall std::runtime_error::runtime_error(runtime_error *this,char *param_1)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZNSt13runtime_errorC1EPKc@@GLIBCXX_3.4.21 */
  halt_baddata();
}



// ======== FT_Get_Kerning @ 012111d0 ========

/* WARNING: Control flow encountered bad instruction data */

void FT_Get_Kerning(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== SDL_GetWindowSize @ 012111d8 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_GetWindowSize(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== string @ 012111e0 ========

/* WARNING: Control flow encountered bad instruction data */

void __thiscall std::__cxx11::string::string(string *this,char *param_1,allocator *param_2)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC2EPKcRKS3_@@GLIBCXX_3.4.21
                        */
  halt_baddata();
}



// ======== __cxa_begin_catch @ 012111e8 ========

/* WARNING: Control flow encountered bad instruction data */

void __cxa_begin_catch(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* __cxa_begin_catch@@CXXABI_1.3 */
  halt_baddata();
}



// ======== SDL_memset @ 012111f0 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_memset(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== SDL_RWFromFile @ 012111f8 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_RWFromFile(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== strlen @ 01211200 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

size_t strlen(char *__s)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* strlen@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== memcmp @ 01211208 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int memcmp(void *__s1,void *__s2,size_t __n)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* memcmp@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== vsprintf @ 01211210 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int vsprintf(char *__s,char *__format,__gnuc_va_list __arg)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* vsprintf@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== mkdir @ 01211218 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int mkdir(char *__path,__mode_t __mode)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* mkdir@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== FNA3D_AddDisposeVertexBuffer @ 01211220 ========

/* WARNING: Control flow encountered bad instruction data */

void FNA3D_AddDisposeVertexBuffer(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== SDL_RWread @ 01211228 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_RWread(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== unlink @ 01211230 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int unlink(char *__name)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* unlink@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== SDL_RWtell @ 01211238 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_RWtell(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== SDL_OpenURL @ 01211240 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_OpenURL(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== SDL_free @ 01211248 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_free(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== __cxa_allocate_exception @ 01211250 ========

/* WARNING: Control flow encountered bad instruction data */

void __cxa_allocate_exception(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* __cxa_allocate_exception@@CXXABI_1.3 */
  halt_baddata();
}



// ======== log10 @ 01211258 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

double log10(double __x)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* log10@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== SDL_GetCurrentDisplayMode @ 01211260 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_GetCurrentDisplayMode(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== difftime @ 01211268 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

double difftime(time_t __time1,time_t __time0)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* difftime@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== _M_transfer @ 01211270 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::__detail::_List_node_base::_M_transfer(_List_node_base *param_1,_List_node_base *param_2)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZNSt8__detail15_List_node_base11_M_transferEPS0_S1_@@GLIBCXX_3.4.15 */
  halt_baddata();
}



// ======== SDL_itoa @ 01211278 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_itoa(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== getcwd @ 01211280 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

char * getcwd(char *__buf,size_t __size)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* getcwd@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== strncpy @ 01211288 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

char * strncpy(char *__dest,char *__src,size_t __n)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* strncpy@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== __throw_length_error @ 01211290 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::__throw_length_error(char *param_1)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZSt20__throw_length_errorPKc@@GLIBCXX_3.4 */
  halt_baddata();
}



// ======== utime @ 01211298 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int utime(char *__file,utimbuf *__file_times)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* utime@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== _M_assign @ 012112a0 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::__cxx11::string::_M_assign(string *param_1)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_assignERKS4_@@GLIBCXX_3.4.21
                        */
  halt_baddata();
}



// ======== memset @ 012112b0 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * memset(void *__s,int __c,size_t __n)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* memset@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== ceilf @ 012112b8 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

float ceilf(float __x)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* ceilf@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== tan @ 012112c8 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

double tan(double __x)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* tan@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== FNA3D_SetVertexBufferData @ 012112d0 ========

/* WARNING: Control flow encountered bad instruction data */

void FNA3D_SetVertexBufferData(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== SDL_strcasecmp @ 012112d8 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_strcasecmp(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== SDL_ShowWindow @ 012112e0 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_ShowWindow(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== readdir64 @ 012112e8 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

dirent64 * readdir64(DIR *__dirp)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* readdir64@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== stb_vorbis_seek_start @ 012112f0 ========

/* WARNING: Control flow encountered bad instruction data */

void stb_vorbis_seek_start(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== _Rb_tree_rebalance_for_erase @ 012112f8 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::_Rb_tree_rebalance_for_erase(_Rb_tree_node_base *param_1,_Rb_tree_node_base *param_2)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZSt28_Rb_tree_rebalance_for_erasePSt18_Rb_tree_node_baseRS_@@GLIBCXX_3.4 */
  halt_baddata();
}



// ======== SDL_PollEvent @ 01211300 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_PollEvent(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== rename @ 01211308 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int rename(char *__old,char *__new)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* rename@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== __cxa_guard_abort @ 01211310 ========

/* WARNING: Control flow encountered bad instruction data */

void __cxa_guard_abort(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* __cxa_guard_abort@@CXXABI_1.3 */
  halt_baddata();
}



// ======== SDL_cos @ 01211318 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_cos(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== __cxa_guard_release @ 01211320 ========

/* WARNING: Control flow encountered bad instruction data */

void __cxa_guard_release(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* __cxa_guard_release@@CXXABI_1.3 */
  halt_baddata();
}



// ======== sysconf @ 01211328 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

long sysconf(int __name)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* sysconf@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== SDL_memcpy @ 01211330 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_memcpy(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== calloc @ 01211338 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * calloc(size_t __nmemb,size_t __size)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* calloc@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== clock @ 01211340 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

clock_t clock(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* clock@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== strncat @ 01211348 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

char * strncat(char *__dest,char *__src,size_t __n)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* strncat@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== mktime @ 01211350 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

time_t mktime(tm *__tp)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* mktime@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== SDL_FilterEvents @ 01211358 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_FilterEvents(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== FNA3D_DrawPrimitives @ 01211360 ========

/* WARNING: Control flow encountered bad instruction data */

void FNA3D_DrawPrimitives(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== FT_Init_FreeType @ 01211368 ========

/* WARNING: Control flow encountered bad instruction data */

void FT_Init_FreeType(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== FNA3D_ResolveTarget @ 01211370 ========

/* WARNING: Control flow encountered bad instruction data */

void FNA3D_ResolveTarget(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== _Rb_tree_increment @ 01211378 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::_Rb_tree_increment(_Rb_tree_node_base *param_1)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZSt18_Rb_tree_incrementPSt18_Rb_tree_node_base@@GLIBCXX_3.4 */
  halt_baddata();
}



// ======== FAudioSourceVoice_Start @ 01211380 ========

/* WARNING: Control flow encountered bad instruction data */

void FAudioSourceVoice_Start(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== __throw_logic_error @ 01211388 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::__throw_logic_error(char *param_1)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZSt19__throw_logic_errorPKc@@GLIBCXX_3.4 */
  halt_baddata();
}



// ======== __cxa_free_exception @ 01211390 ========

/* WARNING: Control flow encountered bad instruction data */

void __cxa_free_exception(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* __cxa_free_exception@@CXXABI_1.3 */
  halt_baddata();
}



// ======== SDL_Delay @ 01211398 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_Delay(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== SDL_vsnprintf @ 012113a0 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_vsnprintf(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== cosf @ 012113a8 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

float cosf(float __x)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* cosf@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== localtime @ 012113b0 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

tm * localtime(time_t *__timer)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* localtime@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== SDL_GetError @ 012113b8 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_GetError(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== FNA3D_ApplyEffect @ 012113c0 ========

/* WARNING: Control flow encountered bad instruction data */

void FNA3D_ApplyEffect(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== stb_vorbis_get_samples_float_interleaved @ 012113c8 ========

/* WARNING: Control flow encountered bad instruction data */

void stb_vorbis_get_samples_float_interleaved(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== __throw_range_error @ 012113d0 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::__throw_range_error(char *param_1)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZSt19__throw_range_errorPKc@@GLIBCXX_3.4 */
  halt_baddata();
}



// ======== FAudioCreate @ 012113d8 ========

/* WARNING: Control flow encountered bad instruction data */

void FAudioCreate(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== strtod @ 012113e0 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

double strtod(char *__nptr,char **__endptr)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* strtod@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== sqrt @ 012113e8 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

double sqrt(double __x)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* sqrt@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== FT_Get_Char_Index @ 012113f0 ========

/* WARNING: Control flow encountered bad instruction data */

void FT_Get_Char_Index(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== SDL_ShowSimpleMessageBox @ 012113f8 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_ShowSimpleMessageBox(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== FNA3D_SetDepthStencilState @ 01211400 ========

/* WARNING: Control flow encountered bad instruction data */

void FNA3D_SetDepthStencilState(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== strftime @ 01211408 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

size_t strftime(char *__s,size_t __maxsize,char *__format,tm *__tp)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* strftime@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== memcpy @ 01211410 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * memcpy(void *__dest,void *__src,size_t __n)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* memcpy@@GLIBC_2.14 */
  halt_baddata();
}



// ======== atan @ 01211418 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

double atan(double __x)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* atan@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== sinf @ 01211420 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

float sinf(float __x)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* sinf@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== FNA3D_Image_Free @ 01211428 ========

/* WARNING: Control flow encountered bad instruction data */

void FNA3D_Image_Free(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== wcslen @ 01211430 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

size_t wcslen(wchar_t *__s)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* wcslen@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== atanf @ 01211438 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

float atanf(float __x)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* atanf@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== system @ 01211440 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int system(char *__command)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* system@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== SDL_PumpEvents @ 01211448 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_PumpEvents(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== gmtime @ 01211450 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

tm * gmtime(time_t *__timer)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* gmtime@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== SDL_isspace @ 01211458 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_isspace(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== __cxa_atexit @ 01211460 ========

/* WARNING: Control flow encountered bad instruction data */

void __cxa_atexit(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* __cxa_atexit@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== FNA3D_Clear @ 01211468 ========

/* WARNING: Control flow encountered bad instruction data */

void FNA3D_Clear(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== setlocale @ 01211470 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

char * setlocale(int __category,char *__locale)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* setlocale@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== SDL_GetPerformanceFrequency @ 01211478 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_GetPerformanceFrequency(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== SDL_Init @ 01211480 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_Init(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== pow @ 01211488 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

double pow(double __x,double __y)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* pow@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== SDL_GameControllerOpen @ 01211490 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_GameControllerOpen(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== SDL_FreeCursor @ 01211498 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_FreeCursor(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== time @ 012114a0 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

time_t time(time_t *__timer)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* time@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== SDL_FreeSurface @ 012114a8 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_FreeSurface(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== strcpy @ 012114b0 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

char * strcpy(char *__dest,char *__src)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* strcpy@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== FNA3D_SetBlendState @ 012114b8 ========

/* WARNING: Control flow encountered bad instruction data */

void FNA3D_SetBlendState(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== acos @ 012114c0 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

double acos(double __x)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* acos@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== operator+= @ 012114c8 ========

/* WARNING: Control flow encountered bad instruction data */

void __thiscall std::__cxx11::string::operator+=(string *this,char *param_1)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEpLEPKc@@GLIBCXX_3.4.21
                        */
  halt_baddata();
}



// ======== srand @ 012114d0 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void srand(uint __seed)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* srand@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== SDL_CreateRGBSurfaceFrom @ 012114d8 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_CreateRGBSurfaceFrom(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== fclose @ 012114e0 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int fclose(FILE *__stream)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* fclose@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== __xstat @ 012114e8 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int __xstat(int __ver,char *__filename,stat *__stat_buf)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* __xstat@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== _M_create @ 012114f0 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::__cxx11::wstring::_M_create(ulong *param_1,ulong param_2)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZNSt7__cxx1112basic_stringIwSt11char_traitsIwESaIwEE9_M_createERmm@@GLIBCXX_3.4.21
                        */
  halt_baddata();
}



// ======== FNA3D_VerifySampler @ 012114f8 ========

/* WARNING: Control flow encountered bad instruction data */

void FNA3D_VerifySampler(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== stb_vorbis_get_info @ 01211500 ========

/* WARNING: Control flow encountered bad instruction data */

void stb_vorbis_get_info(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== FNA3D_ResetBackbuffer @ 01211508 ========

/* WARNING: Control flow encountered bad instruction data */

void FNA3D_ResetBackbuffer(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== bcmp @ 01211510 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int bcmp(void *__s1,void *__s2,size_t __n)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* bcmp@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== SDL_GetTicks @ 01211518 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_GetTicks(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== _Rb_tree_decrement @ 01211520 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::_Rb_tree_decrement(_Rb_tree_node_base *param_1)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZSt18_Rb_tree_decrementPSt18_Rb_tree_node_base@@GLIBCXX_3.4 */
  halt_baddata();
}



// ======== acosf @ 01211528 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

float acosf(float __x)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* acosf@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== fdopen @ 01211530 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

FILE * fdopen(int __fd,char *__modes)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* fdopen@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== FNA3D_SetScissorRect @ 01211538 ========

/* WARNING: Control flow encountered bad instruction data */

void FNA3D_SetScissorRect(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== SDL_GameControllerClose @ 01211540 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_GameControllerClose(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== _M_append @ 01211548 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::__cxx11::wstring::_M_append(wchar_t *param_1,ulong param_2)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZNSt7__cxx1112basic_stringIwSt11char_traitsIwESaIwEE9_M_appendEPKwm@@GLIBCXX_3.4.21
                        */
  halt_baddata();
}



// ======== FAudioVoice_DestroyVoice @ 01211550 ========

/* WARNING: Control flow encountered bad instruction data */

void FAudioVoice_DestroyVoice(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== SDL_GetHintBoolean @ 01211558 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_GetHintBoolean(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== __ctype_b_loc @ 01211560 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

ushort ** __ctype_b_loc(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* __ctype_b_loc@@GLIBC_2.3 */
  halt_baddata();
}



// ======== SDL_RWseek @ 01211568 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_RWseek(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== FNA3D_ApplyRasterizerState @ 01211570 ========

/* WARNING: Control flow encountered bad instruction data */

void FNA3D_ApplyRasterizerState(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== fseek @ 01211578 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int fseek(FILE *__stream,long __off,int __whence)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* fseek@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== memchr @ 01211580 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * memchr(void *__s,int __c,size_t __n)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* memchr@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== _M_reverse @ 01211588 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::__detail::_List_node_base::_M_reverse(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZNSt8__detail15_List_node_base10_M_reverseEv@@GLIBCXX_3.4.15 */
  halt_baddata();
}



// ======== fputs @ 01211590 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int fputs(char *__s,FILE *__stream)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* fputs@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== __dynamic_cast @ 01211598 ========

/* WARNING: Control flow encountered bad instruction data */

void __dynamic_cast(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* __dynamic_cast@@CXXABI_1.3 */
  halt_baddata();
}



// ======== SDL_SetWindowFullscreen @ 012115a0 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_SetWindowFullscreen(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== __lxstat64 @ 012115a8 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int __lxstat64(int __ver,char *__filename,stat64 *__stat_buf)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* __lxstat64@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== FNA3D_AddDisposeTexture @ 012115b0 ========

/* WARNING: Control flow encountered bad instruction data */

void FNA3D_AddDisposeTexture(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== __ostream_insert<char,std::char_traits<char>> @ 012115b8 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

ostream * std::__ostream_insert<char,std::char_traits<char>>
                    (ostream *param_1,char *param_2,long param_3)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@@GLIBCXX_3.4.9
                        */
  halt_baddata();
}



// ======== SDL_pow @ 012115c0 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_pow(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== _Rb_tree_increment @ 012115c8 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::_Rb_tree_increment(_Rb_tree_node_base *param_1)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZSt18_Rb_tree_incrementPKSt18_Rb_tree_node_base@@GLIBCXX_3.4 */
  halt_baddata();
}



// ======== SDL_SetWindowIcon @ 012115d0 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_SetWindowIcon(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== fflush @ 012115d8 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int fflush(FILE *__stream)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* fflush@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== fopen @ 012115e0 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

FILE * fopen(char *__filename,char *__modes)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* fopen@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== _M_erase @ 012115e8 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::__cxx11::string::_M_erase(ulong param_1,ulong param_2)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE8_M_eraseEmm@@GLIBCXX_3.4.21
                        */
  halt_baddata();
}



// ======== free @ 012115f0 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void free(void *__ptr)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* free@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== exit @ 012115f8 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void exit(int __status)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* exit@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== getenv @ 01211600 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

char * getenv(char *__name)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* getenv@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== SDL_JoystickGetVendor @ 01211608 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_JoystickGetVendor(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== _M_mutate @ 01211610 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::__cxx11::wstring::_M_mutate(ulong param_1,ulong param_2,wchar_t *param_3,ulong param_4)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZNSt7__cxx1112basic_stringIwSt11char_traitsIwESaIwEE9_M_mutateEmmPKwm@@GLIBCXX_3.4.21
                        */
  halt_baddata();
}



// ======== SDL_realloc @ 01211618 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_realloc(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== fputc @ 01211620 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int fputc(int __c,FILE *__stream)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* fputc@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== runtime_error @ 01211628 ========

/* WARNING: Control flow encountered bad instruction data */

void __thiscall std::runtime_error::runtime_error(runtime_error *this,string *param_1)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZNSt13runtime_errorC2ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@@GLIBCXX_3.4.21
                        */
  halt_baddata();
}



// ======== SDL_LoadFile @ 01211630 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_LoadFile(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== FNA3D_Image_Load @ 01211638 ========

/* WARNING: Control flow encountered bad instruction data */

void FNA3D_Image_Load(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== SDL_GetWindowFlags @ 01211640 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_GetWindowFlags(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== SDL_RWFromConstMem @ 01211648 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_RWFromConstMem(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== runtime_error @ 01211650 ========

/* WARNING: Control flow encountered bad instruction data */

void __thiscall std::runtime_error::runtime_error(runtime_error *this,runtime_error *param_1)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZNSt13runtime_errorC2ERKS_@@GLIBCXX_3.4.21 */
  halt_baddata();
}



// ======== find @ 01211658 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::__cxx11::string::find(char param_1,ulong param_2)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE4findEcm@@GLIBCXX_3.4.21
                        */
  halt_baddata();
}



// ======== compare @ 01211660 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::__cxx11::wstring::compare(wchar_t *param_1)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZNKSt7__cxx1112basic_stringIwSt11char_traitsIwESaIwEE7compareEPKw@@GLIBCXX_3.4.21
                        */
  halt_baddata();
}



// ======== FNA3D_SwapBuffers @ 01211668 ========

/* WARNING: Control flow encountered bad instruction data */

void FNA3D_SwapBuffers(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== _M_need_rehash @ 01211670 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::__detail::_Prime_rehash_policy::_M_need_rehash(ulong param_1,ulong param_2,ulong param_3)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZNKSt8__detail20_Prime_rehash_policy14_M_need_rehashEmmm@@GLIBCXX_3.4.18 */
  halt_baddata();
}



// ======== exp @ 01211678 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

double exp(double __x)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* exp@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== malloc @ 01211680 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * malloc(size_t __size)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* malloc@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== fmod @ 01211688 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

double fmod(double __x,double __y)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* fmod@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== SDL_RestoreWindow @ 01211690 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_RestoreWindow(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== FAudio_Release @ 01211698 ========

/* WARNING: Control flow encountered bad instruction data */

void FAudio_Release(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== SDL_strcmp @ 012116a0 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_strcmp(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== SDL_GetPerformanceCounter @ 012116a8 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_GetPerformanceCounter(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== FNA3D_DestroyDevice @ 012116b0 ========

/* WARNING: Control flow encountered bad instruction data */

void FNA3D_DestroyDevice(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== tmpfile @ 012116b8 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

FILE * tmpfile(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* tmpfile@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== SDL_ReadLE16 @ 012116c0 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_ReadLE16(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== strcmp @ 012116c8 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int strcmp(char *__s1,char *__s2)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* strcmp@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== strtol @ 012116d0 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

long strtol(char *__nptr,char **__endptr,int __base)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* strtol@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== pclose @ 012116d8 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pclose(FILE *__stream)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* pclose@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== __xstat64 @ 012116e0 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int __xstat64(int __ver,char *__filename,stat64 *__stat_buf)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* __xstat64@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== ~bad_array_new_length @ 012116e8 ========

/* WARNING: Control flow encountered bad instruction data */

void __thiscall std::bad_array_new_length::~bad_array_new_length(bad_array_new_length *this)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZNSt20bad_array_new_lengthD1Ev@@CXXABI_1.3.8 */
  halt_baddata();
}



// ======== fprintf @ 012116f0 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int fprintf(FILE *__stream,char *__format,...)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* fprintf@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== SDL_ReadU8 @ 012116f8 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_ReadU8(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== read @ 01211700 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

ssize_t read(int __fd,void *__buf,size_t __nbytes)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* read@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== _setjmp @ 01211708 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int _setjmp(__jmp_buf_tag *__env)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _setjmp@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== SDL_SetCursor @ 01211710 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_SetCursor(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== FAudio_CreateSourceVoice @ 01211718 ========

/* WARNING: Control flow encountered bad instruction data */

void FAudio_CreateSourceVoice(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== tolower @ 01211720 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int tolower(int __c)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* tolower@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== SDL_strncmp @ 01211728 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_strncmp(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== __cxa_rethrow @ 01211730 ========

/* WARNING: Control flow encountered bad instruction data */

void __cxa_rethrow(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* __cxa_rethrow@@CXXABI_1.3 */
  halt_baddata();
}



// ======== fread @ 01211738 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

size_t fread(void *__ptr,size_t __size,size_t __n,FILE *__stream)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* fread@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== opendir @ 01211740 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

DIR * opendir(char *__name)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* opendir@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== _M_assign @ 01211748 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::__cxx11::wstring::_M_assign(wstring *param_1)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZNSt7__cxx1112basic_stringIwSt11char_traitsIwESaIwEE9_M_assignERKS4_@@GLIBCXX_3.4.21
                        */
  halt_baddata();
}



// ======== __throw_out_of_range_fmt @ 01211750 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::__throw_out_of_range_fmt(char *param_1,...)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZSt24__throw_out_of_range_fmtPKcz@@GLIBCXX_3.4.20 */
  halt_baddata();
}



// ======== find_first_of @ 01211758 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::__cxx11::string::find_first_of(char *param_1,ulong param_2,ulong param_3)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE13find_first_ofEPKcmm@@GLIBCXX_3.4.21
                        */
  halt_baddata();
}



// ======== ldexp @ 01211760 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

double ldexp(double __x,int __exponent)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* ldexp@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== stb_vorbis_close @ 01211768 ========

/* WARNING: Control flow encountered bad instruction data */

void stb_vorbis_close(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== SDL_snprintf @ 01211770 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_snprintf(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== readdir @ 01211778 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

dirent * readdir(DIR *__dirp)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* readdir@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== Init @ 01211780 ========

/* WARNING: Control flow encountered bad instruction data */

void __thiscall std::ios_base::Init::Init(Init *this)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZNSt8ios_base4InitC1Ev@@GLIBCXX_3.4 */
  halt_baddata();
}



// ======== _M_replace @ 01211788 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::__cxx11::string::_M_replace(ulong param_1,ulong param_2,char *param_3,ulong param_4)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_replaceEmmPKcm@@GLIBCXX_3.4.21
                        */
  halt_baddata();
}



// ======== feof @ 01211790 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int feof(FILE *__stream)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* feof@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== powf @ 01211798 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

float powf(float __x,float __y)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* powf@@GLIBC_2.27 */
  halt_baddata();
}



// ======== reserve @ 012117a0 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::__cxx11::string::reserve(ulong param_1)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE7reserveEm@@GLIBCXX_3.4.21
                        */
  halt_baddata();
}



// ======== fgets @ 012117a8 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

char * fgets(char *__s,int __n,FILE *__stream)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* fgets@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== FT_Done_FreeType @ 012117b0 ========

/* WARNING: Control flow encountered bad instruction data */

void FT_Done_FreeType(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== stb_vorbis_open_filename @ 012117b8 ========

/* WARNING: Control flow encountered bad instruction data */

void stb_vorbis_open_filename(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== isalpha @ 012117c0 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int isalpha(int param_1)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* isalpha@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== closedir @ 012117c8 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int closedir(DIR *__dirp)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* closedir@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== SDL_GameControllerMapping @ 012117d0 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_GameControllerMapping(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== SDL_ClearError @ 012117d8 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_ClearError(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== memmove @ 012117e0 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * memmove(void *__dest,void *__src,size_t __n)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* memmove@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== _M_unhook @ 012117e8 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::__detail::_List_node_base::_M_unhook(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZNSt8__detail15_List_node_base9_M_unhookEv@@GLIBCXX_3.4.15 */
  halt_baddata();
}



// ======== SDL_sin @ 012117f0 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_sin(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== __cxa_end_catch @ 012117f8 ========

/* WARNING: Control flow encountered bad instruction data */

void __cxa_end_catch(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* __cxa_end_catch@@CXXABI_1.3 */
  halt_baddata();
}



// ======== realloc @ 01211800 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * realloc(void *__ptr,size_t __size)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* realloc@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== FT_Set_Char_Size @ 01211808 ========

/* WARNING: Control flow encountered bad instruction data */

void FT_Set_Char_Size(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== asin @ 01211810 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

double asin(double __x)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* asin@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== FAudioSourceVoice_SubmitSourceBuffer @ 01211818 ========

/* WARNING: Control flow encountered bad instruction data */

void FAudioSourceVoice_SubmitSourceBuffer(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== SDL_GetWindowDisplayIndex @ 01211820 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_GetWindowDisplayIndex(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== FNA3D_GetTextureData2D @ 01211828 ========

/* WARNING: Control flow encountered bad instruction data */

void FNA3D_GetTextureData2D(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== SDL_ReadLE32 @ 01211830 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_ReadLE32(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== FAudioSourceVoice_GetState @ 01211838 ========

/* WARNING: Control flow encountered bad instruction data */

void FAudioSourceVoice_GetState(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== SDL_SetWindowSize @ 01211840 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_SetWindowSize(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== floor @ 01211848 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

double floor(double __x)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* floor@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== find_first_not_of @ 01211850 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::__cxx11::string::find_first_not_of(char *param_1,ulong param_2,ulong param_3)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE17find_first_not_ofEPKcmm@@GLIBCXX_3.4.21
                        */
  halt_baddata();
}



// ======== clear @ 01211858 ========

/* WARNING: Control flow encountered bad instruction data */

void std::ios::clear(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZNSt9basic_iosIcSt11char_traitsIcEE5clearESt12_Ios_Iostate@@GLIBCXX_3.4 */
  halt_baddata();
}



// ======== __cxa_throw @ 01211860 ========

/* WARNING: Control flow encountered bad instruction data */

void __cxa_throw(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* __cxa_throw@@CXXABI_1.3 */
  halt_baddata();
}



// ======== open64 @ 01211868 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int open64(char *__file,int __oflag,...)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* open64@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== find @ 01211870 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::__cxx11::string::find(char *param_1,ulong param_2,ulong param_3)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE4findEPKcmm@@GLIBCXX_3.4.21
                        */
  halt_baddata();
}



// ======== FNA3D_SetTextureData2D @ 01211878 ========

/* WARNING: Control flow encountered bad instruction data */

void FNA3D_SetTextureData2D(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== SDL_JoystickGetProduct @ 01211880 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_JoystickGetProduct(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== _ITM_deregisterTMCloneTable @ 01211888 ========

/* WARNING: Control flow encountered bad instruction data */

void _ITM_deregisterTMCloneTable(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== find @ 01211890 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::__cxx11::wstring::find(wchar_t *param_1,ulong param_2,ulong param_3)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZNKSt7__cxx1112basic_stringIwSt11char_traitsIwESaIwEE4findEPKwmm@@GLIBCXX_3.4.21
                        */
  halt_baddata();
}



// ======== strpbrk @ 01211898 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

char * strpbrk(char *__s,char *__accept)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* strpbrk@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== SDL_PushEvent @ 012118a0 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_PushEvent(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== _Unwind_Resume @ 012118a8 ========

/* WARNING: Control flow encountered bad instruction data */

void _Unwind_Resume(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _Unwind_Resume@@GCC_3.0 */
  halt_baddata();
}



// ======== FNA3D_CreateEffect @ 012118b0 ========

/* WARNING: Control flow encountered bad instruction data */

void FNA3D_CreateEffect(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== FT_New_Face @ 012118b8 ========

/* WARNING: Control flow encountered bad instruction data */

void FT_New_Face(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== isprint @ 012118c0 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int isprint(int param_1)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* isprint@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== FNA3D_ApplyVertexBufferBindings @ 012118c8 ========

/* WARNING: Control flow encountered bad instruction data */

void FNA3D_ApplyVertexBufferBindings(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== strcat @ 012118d0 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

char * strcat(char *__dest,char *__src)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* strcat@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== FT_Render_Glyph @ 012118d8 ========

/* WARNING: Control flow encountered bad instruction data */

void FT_Render_Glyph(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== _M_create @ 012118e0 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::__cxx11::string::_M_create(ulong *param_1,ulong param_2)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm@@GLIBCXX_3.4.21
                        */
  halt_baddata();
}



// ======== _M_replace @ 012118e8 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::__cxx11::wstring::_M_replace(ulong param_1,ulong param_2,wchar_t *param_3,ulong param_4)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZNSt7__cxx1112basic_stringIwSt11char_traitsIwESaIwEE10_M_replaceEmmPKwm@@GLIBCXX_3.4.21
                        */
  halt_baddata();
}



// ======== remove @ 012118f0 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int remove(char *__filename)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* remove@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== FNA3D_CreateDevice @ 012118f8 ========

/* WARNING: Control flow encountered bad instruction data */

void FNA3D_CreateDevice(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== swap @ 01211900 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::__detail::_List_node_base::swap(_List_node_base *param_1,_List_node_base *param_2)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZNSt8__detail15_List_node_base4swapERS0_S1_@@GLIBCXX_3.4.15 */
  halt_baddata();
}



// ======== _M_mutate @ 01211908 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::__cxx11::string::_M_mutate(ulong param_1,ulong param_2,char *param_3,ulong param_4)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_mutateEmmPKcm@@GLIBCXX_3.4.21
                        */
  halt_baddata();
}



// ======== __cxa_guard_acquire @ 01211910 ========

/* WARNING: Control flow encountered bad instruction data */

void __cxa_guard_acquire(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* __cxa_guard_acquire@@CXXABI_1.3 */
  halt_baddata();
}



// ======== sqrtf @ 01211918 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

float sqrtf(float __x)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* sqrtf@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== __isoc99_fscanf @ 01211920 ========

/* WARNING: Control flow encountered bad instruction data */

void __isoc99_fscanf(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* __isoc99_fscanf@@GLIBC_2.7 */
  halt_baddata();
}



// ======== toupper @ 01211928 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int toupper(int __c)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* toupper@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== SDL_GameControllerGetJoystick @ 01211930 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_GameControllerGetJoystick(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== frexp @ 01211938 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

double frexp(double __x,int *__exponent)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* frexp@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== _M_replace_aux @ 01211940 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::__cxx11::string::_M_replace_aux(ulong param_1,ulong param_2,ulong param_3,char param_4)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE14_M_replace_auxEmmmc@@GLIBCXX_3.4.21
                        */
  halt_baddata();
}



// ======== FT_Done_Face @ 01211948 ========

/* WARNING: Control flow encountered bad instruction data */

void FT_Done_Face(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== cos @ 01211950 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

double cos(double __x)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* cos@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== __libc_start_main @ 01211958 ========

/* WARNING: Control flow encountered bad instruction data */

void __libc_start_main(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* __libc_start_main@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== resize @ 01211960 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::__cxx11::wstring::resize(ulong param_1,wchar_t param_2)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZNSt7__cxx1112basic_stringIwSt11char_traitsIwESaIwEE6resizeEmw@@GLIBCXX_3.4.21
                        */
  halt_baddata();
}



// ======== ferror @ 01211968 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int ferror(FILE *__stream)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* ferror@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== SDL_CreateColorCursor @ 01211970 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_CreateColorCursor(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== runtime_error @ 01211978 ========

/* WARNING: Control flow encountered bad instruction data */

void __thiscall std::runtime_error::runtime_error(runtime_error *this,char *param_1)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZNSt13runtime_errorC2EPKc@@GLIBCXX_3.4.21 */
  halt_baddata();
}



// ======== __gmon_start__ @ 01211980 ========

/* WARNING: Control flow encountered bad instruction data */

void __gmon_start__(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== SDL_GetBasePath @ 01211988 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_GetBasePath(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== FNA3D_PrepareWindowAttributes @ 01211990 ========

/* WARNING: Control flow encountered bad instruction data */

void FNA3D_PrepareWindowAttributes(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== _ITM_registerTMCloneTable @ 01211998 ========

/* WARNING: Control flow encountered bad instruction data */

void _ITM_registerTMCloneTable(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== FNA3D_CreateTexture2D @ 012119a0 ========

/* WARNING: Control flow encountered bad instruction data */

void FNA3D_CreateTexture2D(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== fwrite @ 012119a8 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

size_t fwrite(void *__ptr,size_t __size,size_t __n,FILE *__s)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* fwrite@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== wmemcpy @ 012119b0 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

wchar_t * wmemcpy(wchar_t *__s1,wchar_t *__s2,size_t __n)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* wmemcpy@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== close @ 012119b8 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int close(int __fd)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* close@@GLIBC_2.2.5 */
  halt_baddata();
}



// ======== SDL_ShowCursor @ 012119c0 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_ShowCursor(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== SDL_GameControllerName @ 012119c8 ========

/* WARNING: Control flow encountered bad instruction data */

void SDL_GameControllerName(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ======== rfind @ 012119d0 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::__cxx11::string::rfind(char param_1,ulong param_2)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE5rfindEcm@@GLIBCXX_3.4.21
                        */
  halt_baddata();
}



// ======== operator.new @ 012119d8 ========

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * operator_new(ulong param_1,nothrow_t *param_2)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* _ZnwmRKSt9nothrow_t@@GLIBCXX_3.4 */
  halt_baddata();
}



