/* 割り込み関係 */

#include "bootpack.h"

void init_pic(void)
/* PICの初期化 */
{
  io_out8(PIC0_IMR,  0xff  ); /* 全ての割り込みを受け付けない */
  io_out8(PIC1_IMR,  0xff  ); /* 全ての割り込みを受け付けない */

  io_out8(PIC0_ICW1, 0x11  ); /* エッジトリガモード */
  io_out8(PIC0_ICW2, 0x20  ); /* IRQ0-7は、INT20-27で受ける */
  io_out8(PIC0_ICW1, 1 << 2); /* PIC1はIRQ2に接続 */
  io_out8(PIC0_ICW4, 0x01  ); /* ノンバッファモード */

  io_out8(PIC0_ICW1, 0x11  ); /* エッジトリガモード */
  io_out8(PIC0_ICW2, 0x28  ); /* IRQ8-15は、INT28-2fで受ける */
  io_out8(PIC0_ICW1, 2     ); /* PIC1はIRQ2にて接続 */
  io_out8(PIC0_ICW4, 0x01  ); /* ノンバッファモード */

  io_out8(PIC0_IMR,  0xfb  ); /* 11111011 PIC1以外は全て禁止 */
  io_out8(PIC1_IMR,  0xff  ); /* 11111111 全ての割り込みを受け付けない */

  return;
}

void inthandler21(int *esp)
/* PS/2キーボードからの割り込み */
{
  struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
  boxfill8(binfo->vram, binfo->scrnx, COL8_000000, 0, 0, 32 * 8 - 1, 15);
  putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, "INT 21 (IRQ-1) : PS/2 keyboard");
  for (;;) {
    io_hlt();
  }
}

void inthandler2c(int *esp)
/* PS/2マウスからの割り込み */
{
  struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
  boxfill8(binfo->vram, binfo->scrnx, COL8_000000, 0, 0, 32 * 8 - 1, 15);
  putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, "INT 2c (IRQ-12) : PS/2 mouse");
  for (;;) {
    io_hlt();
  }
}

void inthandler27(int *esp)
/* PIC0からの不完全割り込み対策 */
/* Athlon64X2機などではチップセットの都合によりPICの初期化時にこの割り込みが1度だけおこる */
/* この割り込み処理関数は、その割り込みに対して何もしないでやり過ごす */
/* なぜ何もしなくていいの？ */
/* → この割り込みはPIC初期化時の電気的なノイズによって発生したものなので、  
 *   まじめに何か処理してやる必要はない。
 * */
{
  io_out8(PIC0_OCW2, 0x67); // IRQ-07 受付完了をPICに通知(7-1参照)
  return;
}

