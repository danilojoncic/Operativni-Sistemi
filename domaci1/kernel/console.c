// Console input and output.
// Input is from the keyboard or serial port.
// Output is written to the screen and serial port.

#include "types.h"
#include "defs.h"
#include "param.h"
#include "traps.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "fs.h"
#include "file.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"

static void consputc(int);

static int panicked = 0;
static int currPos;
static int topPos;
static int menuMode = 0;
static int beforeString[81]; //zove se beforeString a zapravo je niz intova,
static int red = 1;
static int color = 0x0700;

static struct {
	struct spinlock lock;
	int locking;
} cons;

static void
printint(int xx, int base, int sign)
{
	static char digits[] = "0123456789abcdef";
	char buf[16];
	int i;
	uint x;

	if(sign && (sign = xx < 0))
		x = -xx;
	else
		x = xx;

	i = 0;
	do{
		buf[i++] = digits[x % base];
	}while((x /= base) != 0);

	if(sign)
		buf[i++] = '-';

	while(--i >= 0)
		consputc(buf[i]);
}

// Print to the console. only understands %d, %x, %p, %s.
void
cprintf(char *fmt, ...)
{
	int i, c, locking;
	uint *argp;
	char *s;

	locking = cons.locking;
	if(locking)
		acquire(&cons.lock);

	if (fmt == 0)
		panic("null fmt");

	argp = (uint*)(void*)(&fmt + 1);
	for(i = 0; (c = fmt[i] & 0xff) != 0; i++){
		if(c != '%'){
			consputc(c);
			continue;
		}
		c = fmt[++i] & 0xff;
		if(c == 0)
			break;
		switch(c){
		case 'd':
			printint(*argp++, 10, 1);
			break;
		case 'x':
		case 'p':
			printint(*argp++, 16, 0);
			break;
		case 's':
			if((s = (char*)*argp++) == 0)
				s = "(null)";
			for(; *s; s++)
				consputc(*s);
			break;
		case '%':
			consputc('%');
			break;
		default:
			// Print unknown % sequence to draw attention.
			consputc('%');
			consputc(c);
			break;
		}
	}

	if(locking)
		release(&cons.lock);
}

void
panic(char *s)
{
	int i;
	uint pcs[10];

	cli();
	cons.locking = 0;
	// use lapiccpunum so that we can call panic from mycpu()
	cprintf("lapicid %d: panic: ", lapicid());
	cprintf(s);
	cprintf("\n");
	getcallerpcs(&s, pcs);
	for(i=0; i<10; i++)
		cprintf(" %p", pcs[i]);
	panicked = 1; // freeze other CPU
	for(;;)
		;
}

#define BACKSPACE 0x100
#define CRTPORT 0x3d4
static ushort *crt = (ushort*)P2V(0xb8000);  // CGA memory

static void
cgaputc(int c)
{
	int pos;
	//jedan switch case za boje zavisno od velicine promijenljive red koja mi predstavlja opciju
	switch(red){
	case 1:
		color = 0x0700;
		break;
	case 3:
		color = 0x7500;
		break;
	case 5:
		color = 0x3400; 
		break;
	case 7:
		color = 0x6700;
		break;
	}

	// Cursor position: col + 80*row.
	outb(CRTPORT, 14);
	pos = inb(CRTPORT+1) << 8;
	outb(CRTPORT, 15);
	pos |= inb(CRTPORT+1);
	
	if(c == '\n')
		pos += 80 - pos%80;
	else if(c == BACKSPACE){
		if(pos > 0) --pos;
	} else
		//color umjesto hex vrijednosti da bi switch case radio da valja
		crt[pos++] = (c&0xff) | color;  // black on white

	if(pos < 0 || pos > 25*80)
		panic("pos under/overflow");

	if((pos/80) >= 24){  // Scroll up.
		memmove(crt, crt+80, sizeof(crt[0])*23*80);
		pos -= 80;
		memset(crt+pos, 0, sizeof(crt[0])*(24*80 - pos));
	}

	outb(CRTPORT, 14);
	outb(CRTPORT+1, pos>>8);
	outb(CRTPORT, 15);
	outb(CRTPORT+1, pos);
	crt[pos] = ' ' | 0x0700;
	currPos = pos;
}

void visualMenuPrint(int currPos){

	char opcije[81] = {'-','-','-','-','-','-','-','-','-','|','W','H','T',' ','B','L','K',
	'|','-','-','-','-','-','-','-','-','-','|','P','U','R',' ','W','H','T',
	'|','-','-','-','-','-','-','-','-','-','|','R','E','D',' ','A','Q','U',
	'|','-','-','-','-','-','-','-','-','-','|','W','H','T',' ','Y','E','L','|','-','-','-','-','-','-','-','-','-'};

	int gornjiLijevo = currPos -(9*80)+1;	//plavi
	int donjiDesno = currPos - 71; 			//ljubicast
	int gornjiDesno = currPos - (9*80)+9;	//crevni
	int donjiLijevo = currPos - 79;			//zeleni
	
	topPos = gornjiLijevo;

	if(((currPos%80) + 9) >=80){
		gornjiLijevo -=10;
		gornjiDesno -= 10;
		donjiDesno -= 10;
		donjiLijevo -=10;

	}
	int begin = gornjiLijevo;
	int finsh = gornjiDesno;	
	int helper = 0;
	while(finsh <= donjiDesno){
		for(int i = begin; i <= finsh;i++){

			//ucitavam sta god da se nalazi u zeljenim lokacijama u beforeString da bi sacuvao
			beforeString[helper] = crt[i];

			//brisem sta se tu nalazi
			crt[i] = crt[i] & 0x0000;

			//stavljam bijelu pozadinu
			crt[i] = crt[i] | 0x7000;

			//stavljam karaktere iz char niza opcije
			crt[i] = crt[i] | opcije[helper];
			helper++;
		}
		finsh += 80;
		begin += 80;
	}

	//debug farbanje
	/*
	crt[gornjiLijevo] &= 0x00ff;
	crt[gornjiLijevo] |= 0x1100;

	crt[donjiDesno] &= 0x00ff;
	crt[donjiDesno] |= 0x5500;

	crt[donjiLijevo] &= 0x00ff;
	crt[donjiLijevo] |= 0x2200;

	crt[gornjiDesno] &= 0x00ff;
	crt[gornjiDesno] |= 0x4400;
	*/
}

void refresh(){
	int helper = 0;
	//lokalna promijenljiva topPos2
	int topPos2 = topPos;
	if((currPos%80 + 9) >= 80){
		topPos2-=10;
	}
	for(int i = topPos2; i <= (topPos2 + 8*80 +8); i++){
		if((i%80 >= topPos2%80) && i%80 <= (topPos2+8)%80){
			crt[i] = crt[i] & 0x0000;
			crt[i] = crt[i] | beforeString[helper++];
		}
	}
}

void refreshOption(int red){
	//lokalna promijenljiva topPos2;
	int topPos2 = topPos;
	if((currPos%80 + 9) >= 80){
		topPos2-=10;
	}
	for(int i = topPos2+1; i < currPos-80;i++){
		if((i%80 >= topPos2%80) && i%80 < (topPos2+8)%80){
			crt[i] = crt[i] & 0x00ff;
			crt[i] = crt[i] | 0x7000;
		}
	}
	for(int i = topPos2+1; i <= currPos-80;i++){
		if(i > topPos2 + (red*80) && i < topPos2 + (red*80)+8){
			crt[i] = crt[i] & 0x00ff;
			crt[i] = crt[i] | 0x2000;
		}		
	}
}

void
consputc(int c)
{
	if(panicked){
		cli();
		for(;;)
			;
	}
	//u slucaju da udjem u meni mod, blokiram ovu funkciju
	if(menuMode){
		return;
	}

	if(c == BACKSPACE){
		uartputc('\b'); uartputc(' '); uartputc('\b');
	} else
		uartputc(c);
	cgaputc(c);
}

#define INPUT_BUF 128
struct {
	char buf[INPUT_BUF];
	uint r;  // Read index
	uint w;  // Write index
	uint e;  // Edit index
} input;

#define C(x)  ((x)-'@')  // Control-x
#define A(x) ((x) - 60) //ALT+X

void
consoleintr(int (*getc)(void))
{
	int c, doprocdump = 0;

	acquire(&cons.lock);
	while((c = getc()) >= 0){
		switch(c){
		case C('P'):  // Process listing.
			// procdump() locks cons.lock indirectly; invoke later
			doprocdump = 1;
			break;
		case C('U'):  // Kill line.
			while(input.e != input.w &&
			      input.buf[(input.e-1) % INPUT_BUF] != '\n'){
				input.e--;
				consputc(BACKSPACE);
			}
			break;
		case C('H'): case '\x7f':  // Backspace
			if(input.e != input.w){
				input.e--;
				consputc(BACKSPACE);
			}
			break;
		case A('C'): 
			if(menuMode == 0){
				menuMode = 1;
				visualMenuPrint(currPos);
				refreshOption(red);
			}else{
				menuMode = 0;
				refresh();
			}
			break;
		default:
			if(menuMode){
				if(c == 'w'){
				   red-=2;
				   if(red < 1){
						red = 7;
						refreshOption(red);
					}
					refreshOption(red);
				}else if(c == 's'){
					red+=2;
					if(red > 8){
						red = 1;
						refreshOption(red);
					}
					refreshOption(red);
				}
			}else if(c != 0 && input.e-input.r < INPUT_BUF){
				c = (c == '\r') ? '\n' : c;
				input.buf[input.e++ % INPUT_BUF] = c;
				consputc(c);
				if(c == '\n' || c == C('D') || input.e == input.r+INPUT_BUF){
					input.w = input.e;
					wakeup(&input.r);
				}
			}
			break;
		}
	}
	release(&cons.lock);
	if(doprocdump) {
		procdump();  // now call procdump() wo. cons.lock held
	}
}

int
consoleread(struct inode *ip, char *dst, int n)
{
	uint target;
	int c;

	iunlock(ip);
	target = n;
	acquire(&cons.lock);
	while(n > 0){
		while(input.r == input.w){
			if(myproc()->killed){
				release(&cons.lock);
				ilock(ip);
				return -1;
			}
			sleep(&input.r, &cons.lock);
		}
		c = input.buf[input.r++ % INPUT_BUF];
		if(c == C('D')){  // EOF
			if(n < target){
				// Save ^D for next time, to make sure
				// caller gets a 0-byte result.
				input.r--;
			}
			break;
		}
		*dst++ = c;
		--n;
		if(c == '\n')
			break;
	}
	release(&cons.lock);
	ilock(ip);

	return target - n;
}

int
consolewrite(struct inode *ip, char *buf, int n)
{
	int i;

	iunlock(ip);
	acquire(&cons.lock);
	for(i = 0; i < n; i++)
		consputc(buf[i] & 0xff);
	release(&cons.lock);
	ilock(ip);

	return n;
}

void
consoleinit(void)
{
	initlock(&cons.lock, "console");

	devsw[CONSOLE].write = consolewrite;
	devsw[CONSOLE].read = consoleread;
	cons.locking = 1;

	ioapicenable(IRQ_KBD, 0);
}

