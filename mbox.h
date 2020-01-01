// a properly aligned buffer
extern volatile unsigned int mbox[36];

#define MBOX_REQUEST    0

// channels
#define MBOX_CH_POWER   0
#define MBOX_CH_FB      1
#define MBOX_CH_VUART   2
#define MBOX_CH_VCHIQ   3
#define MBOX_CH_LEDS    4
#define MBOX_CH_BTNS    5
#define MBOX_CH_TOUCH   6
#define MBOX_CH_COUNT   7
#define MBOX_CH_PROP    8

// tags
#define MBOX_TAG_SETPOWER       0x28001
#define MBOX_TAG_SETCLKRATE     0x38002
#define MBOX_TAG_LAST           0

// color
#define COL8_000000 (((0x00>>3)<<11) + ((0x00>>3)<<6) + (0x00>>3))
#define COL8_008484 (((0x00>>3)<<11) + ((0x84>>3)<<6) + (0x84>>3))
#define COL8_848484 (((0x84>>3)<<11) + ((0x84>>3)<<6) + (0x84>>3))
#define COL8_C6C6C6 (((0xC6>>3)<<11) + ((0xC6>>3)<<6) + (0xC6>>3))
#define COL8_FFFFFF (((0xFF>>3)<<11) + ((0xFF>>3)<<6) + (0xFF>>3))

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

int mbox_call(unsigned char ch);
void mbox_write(unsigned char ch);
int mbox_read(unsigned char ch);
//void lfd_init(fb_info_t *fb_info);
void lfd_init(void);
//static inline void *coord2ptr(unsigned char *vram,unsigned int pitch,unsigned int bpp,int x,int y);
void boxfill8(unsigned char *vram,unsigned int c,unsigned int pitch,unsigned int bpp,int x0, int y0, int x1, int y1);

void lfb_print(int x, int y, char *s);
