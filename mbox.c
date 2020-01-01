#include "gpio.h"

// mailbox message buffer
volatile unsigned int  __attribute__((aligned(16))) mbox[36];

#define VIDEOCORE_MBOX  (MMIO_BASE+0x0000B880)
#define MBOX_READ       ((volatile unsigned int*)(VIDEOCORE_MBOX+0x0))
#define MBOX_POLL       ((volatile unsigned int*)(VIDEOCORE_MBOX+0x10))
#define MBOX_SENDER     ((volatile unsigned int*)(VIDEOCORE_MBOX+0x14))
#define MBOX_STATUS     ((volatile unsigned int*)(VIDEOCORE_MBOX+0x18))
#define MBOX_CONFIG     ((volatile unsigned int*)(VIDEOCORE_MBOX+0x1C))
#define MBOX_WRITE      ((volatile unsigned int*)(VIDEOCORE_MBOX+0x20))
#define MBOX_RESPONSE   0x80000000
#define MBOX_FULL       0x80000000
#define MBOX_EMPTY      0x40000000

// request
#define MBOX_REQUEST    0
// channels
#define MBOX_CH_PROP    8
// tags
#define MBOX_TAG_LAST   0

// Make a mailbox call. Returns 0 on failure, non-zero on success
int mbox_call(unsigned char ch) {
    unsigned int r = (((unsigned int)((unsigned long)&mbox)&~0xF) | (ch&0xF));
    // wait until we can write to the mailbox
    do{asm volatile("nop");}while(*MBOX_STATUS & MBOX_FULL);
    // write the address of our message to the mailbox with channel identifier
    *MBOX_WRITE = r;
    // now wait for the response
    while(1) {
        // is there a response?
        do{asm volatile("nop");}while(*MBOX_STATUS & MBOX_EMPTY);
        // is it a response to our message?
        if(r == *MBOX_READ)
            // is it a valid successful response?
            return mbox[1]==MBOX_RESPONSE;
    }
    return 0;
}

void mbox_write(unsigned char ch){
    unsigned int r = (((unsigned int)((unsigned long)&mbox) & ~0xF) | (ch & 0xF));

    // メールボックスに書き込むまで待つ
    do{
        asm volatile("nop");
    }while(*MBOX_STATUS & MBOX_FULL);

    // メッセージのアドレスをチャネル識別子を持つメールボックスに書き込む
    *MBOX_WRITE = r;
}

int mbox_read(unsigned char ch){
    unsigned int r = (((unsigned int)((unsigned long)&mbox) & ~0xF) | (ch & 0xF));
    while(1) {
        // 応答を待つ
        do{
            asm volatile("nop");
        }while(*MBOX_STATUS & MBOX_EMPTY);

        // メッセージに対する応答か判定
        if(r == *MBOX_READ){
            // 有効な成功応答か判定
            return mbox[1] == MBOX_RESPONSE;
        }
    }
    return 0;
}

/*

typedef struct _fb_info_t {
    unsigned int  display_w;  // display width
    unsigned int  display_h;  // display height
    unsigned int  w;          // framebuffer width
    unsigned int  h;          // framebuffer height
    unsigned int  row_bytes;  // write 0 to get value
    unsigned int  bpp;        // bits per pixel
    unsigned int  ofs_x;      // x offset of framebuffer
    unsigned int  ofs_y;      // y offset of framebuffer
    unsigned char *buf_addr;  // pointer to framebuffer
    unsigned int  buf_size;   // framebuffer size in bytes
} fb_info_t;


static inline void *coord2ptr(unsigned char *vram,unsigned int pitch,unsigned int bpp,int x,int y) {
    return (void *)(vram + ((bpp + 7) >> 3) * x + pitch * y);
}

void boxfill8(unsigned char *vram,unsigned int c,unsigned int pitch,unsigned int bpp,int x0, int y0, int x1, int y1){
    int x, y;
    unsigned short int *p;
    for (y = y0; y <= y1; y++) {
        for (x = x0; x <= x1; x++){
            p = (unsigned short int *)coord2ptr(vram, pitch, bpp, x, y);
            *p = c;
        }
    }
}


// PC Screen Font as used by Linux Console
typedef struct {
    unsigned int magic;
    unsigned int version;
    unsigned int headersize;
    unsigned int flags;
    unsigned int numglyph;
    unsigned int bytesperglyph;
    unsigned int height;
    unsigned int width;
    unsigned char glyphs;
} __attribute__((packed)) psf_t;
extern volatile unsigned char _binary_font_psf_start;

unsigned int width, height, pitch;
unsigned char *lfb;


// Set screen resolution to 1024x768
void lfb_init()
{
    mbox[0] = 35*4;
    mbox[1] = MBOX_REQUEST;

    mbox[2] = 0x48003;  //set phy wh
    mbox[3] = 8;
    mbox[4] = 8;
    mbox[5] = 1024;         //FrameBufferInfo.width
    mbox[6] = 768;          //FrameBufferInfo.height

    mbox[7] = 0x48004;  //set virt wh
    mbox[8] = 8;
    mbox[9] = 8;
    mbox[10] = 1024;        //FrameBufferInfo.virtual_width
    mbox[11] = 768;         //FrameBufferInfo.virtual_height
    
    mbox[12] = 0x48009; //set virt offset
    mbox[13] = 8;
    mbox[14] = 8;
    mbox[15] = 0;           //FrameBufferInfo.x_offset
    mbox[16] = 0;           //FrameBufferInfo.y.offset
    
    mbox[17] = 0x48005; //set depth
    mbox[18] = 4;
    mbox[19] = 4;
    mbox[20] = 32;          //FrameBufferInfo.depth

    mbox[21] = 0x48006; //set pixel order
    mbox[22] = 4;
    mbox[23] = 4;
    mbox[24] = 1;           //RGB, not BGR preferably

    mbox[25] = 0x40001; //get framebuffer, gets alignment on request
    mbox[26] = 8;
    mbox[27] = 8;
    mbox[28] = 4096;        //FrameBufferInfo.pointer
    mbox[29] = 0;           //FrameBufferInfo.size

    mbox[30] = 0x40008; //get pitch
    mbox[31] = 4;
    mbox[32] = 4;
    mbox[33] = 0;           //FrameBufferInfo.pitch

    mbox[34] = MBOX_TAG_LAST;

    if(mbox_call(MBOX_CH_PROP) && mbox[20]==32 && mbox[28]!=0) {
        mbox[28]&=0x3FFFFFFF;
        width=mbox[5];
        height=mbox[6];
        pitch=mbox[33];
        lfb=(void*)((unsigned long)mbox[28]);
    } else {
        uart_puts("Unable to set screen resolution to 1024x768x32\n");
    }
}

void lfb_print(int x, int y, char *s) {
    // get our font
    psf_t *font = (psf_t*)&_binary_font_psf_start;
    // draw next character if it's not zero
    while(*s) {
        // get the offset of the glyph. Need to adjust this to support unicode table
        unsigned char *glyph = (unsigned char*)&_binary_font_psf_start +
         font->headersize + (*((unsigned char*)s)<font->numglyph?*s:0)*font->bytesperglyph;
        // calculate the offset on screen
        int offs = (y * font->height * pitch) + (x * (font->width+1) * 4);
        // variables
        int i,j, line,mask, bytesperline=(font->width+7)/8;
        // handle carrige return
        if(*s=='\r') {
            x=0;
        } else
        // new line
        if(*s=='\n') {
            x=0; y++;
        } else {
            // display a character
            for(j=0;j<font->height;j++){
                // display one row
                line=offs;
                mask=1<<(font->width-1);
                for(i=0;i<font->width;i++){
                    // if bit set, we use white color, otherwise black
                    *((unsigned int*)(lfb + line))=((int)*glyph) & mask?0xFFFFFF:0;
                    mask>>=1;
                    line+=4;
                }
                // adjust to next line
                glyph+=bytesperline;
                offs+=pitch;
            }
            x++;
        }
        // next character
        s++;
    }
}


*/
