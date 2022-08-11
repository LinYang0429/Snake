/*
CS 349 A1 Skeleton Code - Snake

- - - - - - - - - - - - - - - - - - - - - -

Commands to compile and run:

    g++ -o snake snake.cpp -L/usr/X11R6/lib -lX11 -lstdc++
    ./snake

Note: the -L option and -lstdc++ may not be needed on some machines.
*/

#include <iostream>
#include <list>
#include <cstdlib>
#include <sys/time.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>

/*
 * Header files for X functions
 */
#include <X11/Xlib.h>
#include <X11/Xutil.h>

using namespace std;

/*
 * Global game state variables
 */
const int Border = 1;
const int BufferSize = 10;
int FPS;
int speed;
useconds_t frametime;
const int width = 800;
const int height = 600;
int score = 0;
bool go = false;
bool start = false;

/*
 * Information to draw on the window.
 */
struct XInfo {
	Display	 *display;
	int		 screen;
	Window	 window;
	GC		 gc[3];
	int		width;		// size of window
	int		height;
};


/*
 * Function to put out a message on error exits.
 */
void error( string str ) {
  exit(0);
}


/*
 * An abstract class representing displayable things.
 */
class Displayable {
	public:
		virtual void paint(XInfo &xinfo) = 0;
};

class Block {
	public:
		int x;
		int y;

	Block(int x, int y) : x(x), y(y){
	}

	int getX() {
		return x;
	}

	int getY() {
		return y;
	}
};

class Text : public Displayable {
public:
  virtual void paint(XInfo& xinfo) {
    XDrawImageString( xinfo.display, xinfo.window, xinfo.gc[0],
                      this->x, this->y, this->s.c_str(), this->s.length() );
  }

  // constructor
  Text(int x, int y, string s): x(x), y(y), s(s)  {}
  void setString(string s) {
	  this->s = s;
  }

private:
  int x;
  int y;
  string s; // string to show
};

class Border : public Displayable {
public:
  virtual void paint(XInfo& xinfo) {
	XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], 50, 0, 700, 10);
	XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], 50, 590, 700, 10);
	XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], 0, 50, 10, 500);
	XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], 590, 50, 10, 500);
  }
};


class Fruit : public Displayable {
	public:
		virtual void paint(XInfo &xinfo) {
			XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], x, y, 10, 10);
        }

        Fruit() {
        	srand(time(NULL));
        	do {
        		regenerate();
        	} while (!(x == 50 && y == 40) && !(x == 40 && y == 40));
        }

        int getX() {
        	return x;
        }

        int getY() {
        	return y;
        }

        void regenerate() {
        	x = (rand() % width/10) * 10;
        	y = (rand() % height/10) * 10;
        }

    private:
        int x;
        int y;
};

class Snake : public Displayable {
	public:
		virtual void paint(XInfo &xinfo) {
			 for(std::list<Block>::iterator it=snake.begin(); it!=snake.end(); ++it) {
				XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], (*it).getX(), (*it).getY(), blockSize, blockSize);
			 }
		}

		bool move(XInfo &xinfo) {
			if (!isPause && start) {
				int newHeadX, newHeadY;
				if (head == 0 || head == 2) {
					newHeadX = ((*(snake.begin())).getX() + direction) % width;
					newHeadY = (*(snake.begin())).getY();
				} else {
					newHeadY = ((*(snake.begin())).getY() + direction) & height;
					newHeadX = (*(snake.begin())).getX();
				}

				Block *newHead = new Block(newHeadX, newHeadY);
				snake.push_front(*newHead);
				if (newHeadX == fruit->getX() && newHeadY == fruit->getY()) {
					do {
						fruit.regenerate();
					} while(!snake.checkFruit(fruit.getX(), fruit.getY()));
					score++;
					leftConer->setString("Score: " + to_string(score));
				} else {
					snake.pop_back();
				}

				 // rule of die
				if ((newHeadX <= 0 || newHeadX => 790) && y >= 50 && y < 550)) {
//					cerr << "Hit walls." << endl;
					return false;
				}
				if ((newHeadY <= 0 || newHeadY => 590) && x >= 50 && x < 750) {
//					cerr << "Hit walls." << endl;
	  				return false;
				}
				int count = 0;
				for (std::list<Block>::iterator it=snake.begin(); it!=snake.end(); ++it) {
					if(newHeadX == (*it).getX() && newHeadY == (*it).getY()) {
						count++;
					}
				}
				if(count > 1) {
					return false;
				}
	            return true;
            }
            return true;
		}

		int getX() {
			return x;
		}

		int getY() {
			return y;
		}

		int getHead() {
			return head;
		}

		bool getPauseValue(){
			return isPause;
		}

		bool checkFruit(int x, int y) {
			for (std::list<Block>::iterator it=snake.begin(); it!=snake.end(); ++it) {
				if(x == (*it).getX() && y == (*it).getY()) {
					return false;
				}
			}
			if (x == 0 || x == 790 || y == 0 || y == 590) {
				return false;
			}
			return true;
		}

        /*
         * ** ADD YOUR LOGIC **
         * Use these placeholder methods as guidance for implementing the snake behaviour.
         * You do not have to use these methods, feel free to implement your own.
         */
        void didEatFruit() {
			blockSize += 10;
        }

        void didHitObstacle() {
        }

        void turnLeft() {
        	if (!isPause && start) {
				switch (head) {
					case 0:
						head = 1;
						direction = -direction;
						return;
					case 1:
						head = 2;
						return;
					case 2:
						head = 3;
						direction = -direction;
						return;
					case 3:
						head = 0;
						return;
				}
        	}
        }

        void turnRight() {
        	if (!isPause && start) {
				switch (head) {
					case 0:
						head = 3;
						return;
					case 1:
						head = 0;
						direction = -direction;
						return;
					case 2:
						head = 1;
						return;
					case 3:
						head = 2;
						direction = -direction;
						return;
				}
			}
        }

        void pause() {
        	isPause = !isPause;
        }

        void restart() {
			head = 2;
			snake.clear();
			Block starter = Block(50, 40);
			Block s1 = Block(40, 40);
			snake.push_front(starter);
			snake.push_front(s1);
			isPause = false;
        }

		Snake(Fruit *fruit, Text *leftConer) {
			direction = 10;
            blockSize = 10;
			head = 2;
			Block starter = Block(50, 40);
			Block s1 = Block(40, 40);
			snake.push_front(starter);
			snake.push_front(s1);
			this->fruit = fruit;
			this->leftConer = leftConer;
			isPause = false;
		}

	private:
		int x;
		int y;
		int blockSize;
		int direction;
		int head; // 0: left 1: down 2: right 3: up
		std::list<Block> snake;
		Fruit *fruit;
		Text *leftConer;
		bool isPause;
};

list<Displayable *> dList;           // list of Displayables
Fruit fruit;
Text leftConer(0, 580, "score: 0");
Snake snake(&fruit, &leftConer);
Text rightConer(700, 550, "FPS: 0");
Text rightConer1(700, 580, "speed: 0");
Text gameOver(350, 250, "Game over...");
Text gameOver1(350, 300, "Your score is 0.");
Text gameOver2(350, 350, "Press r to restart. Press q to quit game.");
Text p(350, 260, "Paused!");
Text name(350, 50, "Lin Yang");
Text userid(350, 100, "l76yang");
Text space(350, 200, "Press space to start the game");
Text keys(350, 250, "Use arrow keys or WASD to control the snake's movements");
Text paused(350, 300, "Press p to pause the game");
Text restart(350, 350, "Press r to restart the game");
Text quit(350, 400, "Press q to quit the game");
Border b();

void splashScreen() {
    dList.push_front(&name);
    dList.push_front(&userid);
    dList.push_front(&space);
    dList.push_front(&keys);
    dList.push_front(&paused);
    dList.push_front(&restart);
    dList.push_front(&quit);
}

/*
 * Initialize X and create a window
 */
void initX(int argc, char *argv[], XInfo &xInfo) {
	XSizeHints hints;
	unsigned long white, black;

   /*
	* Display opening uses the DISPLAY	environment variable.
	* It can go wrong if DISPLAY isn't set, or you don't have permission.
	*/
	xInfo.display = XOpenDisplay( "" );
	if ( !xInfo.display )	{
		error( "Can't open display." );
	}

   /*
	* Find out some things about the display you're using.
	*/
	xInfo.screen = DefaultScreen( xInfo.display );

	white = XWhitePixel( xInfo.display, xInfo.screen );
	black = XBlackPixel( xInfo.display, xInfo.screen );

	hints.x = 100;
	hints.y = 100;
	hints.width = 800;
	hints.height = 600;
	hints.flags = PPosition | PSize;

	xInfo.window = XCreateSimpleWindow(
		xInfo.display,				// display where window appears
		DefaultRootWindow( xInfo.display ), // window's parent in window tree
		hints.x, hints.y,			// upper left corner location
		hints.width, hints.height,	// size of the window
		Border,						// width of window's border
		black,						// window border colour
		white );					// window background colour

	XSetStandardProperties(
		xInfo.display,		// display containing the window
		xInfo.window,		// window whose properties are set
		"animation",		// window's title
		"Animate",			// icon's title
		None,				// pixmap for the icon
		argv, argc,			// applications command line args
		&hints );			// size hints for the window

	/*
	 * Create Graphics Contexts
	 */
	int i = 0;
	xInfo.gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);
	XSetForeground(xInfo.display, xInfo.gc[i], BlackPixel(xInfo.display, xInfo.screen));
	XSetBackground(xInfo.display, xInfo.gc[i], WhitePixel(xInfo.display, xInfo.screen));
	XSetFillStyle(xInfo.display, xInfo.gc[i], FillSolid);
	XSetLineAttributes(xInfo.display, xInfo.gc[i],
	                     1, LineSolid, CapButt, JoinRound);

	XSelectInput(xInfo.display, xInfo.window,
		ButtonPressMask | KeyPressMask |
		PointerMotionMask |
		EnterWindowMask | LeaveWindowMask |
		StructureNotifyMask);  // for resize events

	/*
	 * Put the window on the screen.
	 */
	XMapRaised( xInfo.display, xInfo.window );
	XFlush(xInfo.display);
}

/*
 * Function to repaint a display list
 */
void repaint( XInfo &xinfo) {
	list<Displayable *>::const_iterator begin = dList.begin();
	list<Displayable *>::const_iterator end = dList.end();

	XClearWindow( xinfo.display, xinfo.window );

	// get height and width of window (might have changed since last repaint)

	XWindowAttributes windowInfo;
//	XGetWindowAttributes(xinfo.display, xinfo.window, &windowInfo);
	unsigned int height = windowInfo.height;
	unsigned int width = windowInfo.width;

	// big black rectangle to clear background

	// draw display list
	while( begin != end ) {
		Displayable *d = *begin;
		d->paint(xinfo);
		begin++;
	}
	XFlush( xinfo.display );
}

void handleKeyPress(XInfo &xinfo, XEvent &event) {
	KeySym key;
	char text[BufferSize];

	/*
	 * Exit when 'q' is typed.
	 * This is a simplified approach that does NOT use localization.
	 */
	int i = XLookupString(
		(XKeyEvent *)&event, 	// the keyboard event
		text, 					// buffer when text will be written
		BufferSize, 			// size of the text buffer
		&key, 					// workstation-independent key symbol
		NULL );					// pointer to a composeStatus structure (unused)
	if ( i == 1) {
//		printf("Got key press -- %c\n", text[0]);
		if (text[0] == 'q') {
			error("Terminating normally.");
		} else if (text[0] == 'w') {
			switch (snake.getHead()) {
				case 0:
					snake.turnRight();
					return;
				case 2:
					snake.turnLeft();
					return;
				default:
					return;
			}
		} else if (text[0] == 's') {
			switch (snake.getHead()) {
				case 0:
					snake.turnLeft();
					return;
				case 2:
					snake.turnRight();
					return;
				default:
					return;
			}
		} else if (text[0] == 'a') {
			switch (snake.getHead()) {
				case 1:
					snake.turnRight();
					return;
				case 3:
					snake.turnLeft();
					return;
				default:
					return;
			}
		} else if (text[0] == 'd') {
			switch (snake.getHead()) {
				case 1:
					snake.turnLeft();
					return;
				case 3:
					snake.turnRight();
					return;
				default:
					return;
			}
		} else if (text[0] == 'p') {
			if (start && !go) {
				snake.pause();
				if(snake.getPauseValue()) {
					dList.push_front(&p);
				} else {
					dList.remove(&p);
				}
			}
			return;
		} else if (text[0] == 'r') {
			if(snake.getPauseValue()) {
				dList.remove(&p);
			}
			snake.restart();
			do {
				fruit.regenerate();
			} while(!snake.checkFruit(fruit.getX(), fruit.getY()));
			score = 0;
			leftConer.setString("Score: " + to_string(score));
			if (go) {
				dList.clear();
				dList.push_front(&snake);
			    dList.push_front(&fruit);
			    dList.push_front(&leftConer);
			    dList.push_front(&rightConer);
			    dList.push_front(&rightConer1);
			}
		} else if (text[0] == ' ') {
			if (!start) {
				start = true;
    			dList.clear();
				dList.push_front(&snake);
			    dList.push_front(&fruit);
			    dList.push_front(&leftConer);
			    dList.push_front(&rightConer);
			    dList.push_front(&rightConer1);
			    dList.push_front(&b);
			}
		}
	}

	switch(key){
				case XK_Up:
					switch (snake.getHead()) {
						case 0:
							snake.turnRight();
							return;
						case 2:
							snake.turnLeft();
							return;
						default:
						return;
					}
					return;
			    case XK_Down:
			    	switch (snake.getHead()) {
			    		case 0:
			    			snake.turnLeft();
			    			return;
			    		case 2:
			    			snake.turnRight();
			    			return;
			    		default:
			    			return;
			    	}
			    	return;
			    case XK_Left:
			    	switch (snake.getHead()) {
			    		case 1:
			    			snake.turnRight();
			    			return;
			    		case 3:
			    			snake.turnLeft();
			    			return;
			    		default:
			    			return;
			    	}
			    	return;
			    case XK_Right:
			    	switch (snake.getHead()) {
			    		case 1:
			    			snake.turnLeft();
			    			return;
			    		case 3:
			    			snake.turnRight();
			    			return;
			    		default:
			    			return;
			    	}
			    return;
			}
}

void handleAnimation(XInfo &xinfo, int inside, useconds_t *counter) {
    /*
     * ADD YOUR OWN LOGIC
     * This method handles animation for different objects on the screen and readies the next frame before the screen is re-painted.
     */
	useconds_t diff = 1000000 / (speed * 10);
    if (*counter >= diff) {
    	*counter -= diff;
    	if(!snake.move(xinfo)) {
    		// game over
    		go = true;
    		dList.clear();
			dList.push_front(&gameOver);
			gameOver1.setString("Your score is " + to_string(score));
			dList.push_front(&gameOver1);
			dList.push_front(&gameOver2);
    	}
    }
}

// get microseconds
unsigned long now() {
	timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}

void eventLoop(XInfo &xinfo) {
	// Add stuff to paint to the display list
	splashScreen();

	XEvent event;
	unsigned long lastRepaint = 0;
	int inside = 0;

	useconds_t counter = 0;
	while( true ) {
		/*
		 * This is NOT a performant event loop!
		 * It needs help!
		 */

		if (XPending(xinfo.display) > 0) {
			XNextEvent( xinfo.display, &event );
//			cout << "event.type=" << event.type << "\n";
			switch( event.type ) {
				case KeyPress:
					handleKeyPress(xinfo, event);
					break;
				case EnterNotify:
					inside = 1;
					break;
				case LeaveNotify:
					inside = 0;
					break;
			}
		}

		repaint(xinfo);
		usleep(frametime);
		handleAnimation(xinfo, inside, &counter);
		counter += frametime;
	}
}


/*
 * Start executing here.
 *	 First initialize window.
 *	 Next loop responding to events.
 *	 Exit forcing window manager to clean up - cheesy, but easy.
 */
int main ( int argc, char *argv[] ) {
	FPS = 30;
	speed = 5;
	if (argc == 2) {
		FPS = stoi(argv[1]);
	} else if (argc == 3) {
		FPS = stoi(argv[1]);
		speed = stoi(argv[2]);
	}
	frametime = 1000000/FPS;

	XInfo xInfo;

	initX(argc, argv, xInfo);
	rightConer1.setString("Speed: " + to_string(speed));
	rightConer.setString("FPS: " + to_string(FPS));
	eventLoop(xInfo);
	XCloseDisplay(xInfo.display);
}
