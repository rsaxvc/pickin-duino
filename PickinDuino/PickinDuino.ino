#include <TVout.h>
#include <fontALL.h>

//Must be supported by TV module
#define GAME_WIDTH 128
#define GAME_HEIGHT 96

#define PLAYER_WIDTH  7
#define PLAYER_HEIGHT 10

#define STICKS_WIDTH  6
#define STICKS_HEIGHT 7

#define MAX_SCORE 255

TVout TV;
struct point2d
  {
  //data
  unsigned char x;
  unsigned char y;

  //drawing
  void point( unsigned char x0, unsigned char y0, char color )
    {TV.set_pixel( x + x0, y + y0, color );}
  void circle( unsigned char x0, unsigned char y0, unsigned char radius, char color, char fill_color )
    {TV.draw_circle( x + x0, y + y0, radius, color, fill_color ); }
  void line( unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, char color )
    {TV.draw_line( x + x0, y + y0, x + x1, y + y1, color ); }
  };
  
signed char dx;
signed char dy;

point2d player;
point2d sticks;
unsigned char score;

//draw the player
void draw_player( void )
{
player.line( 1, 0, 5, 0, 2);//forehead
player.line( 0, 1, 0, 3, 2);//left cheek
player.line( 6, 1, 6, 3, 2);//right cheek
player.point( 2, 2, 2 );//left eye
player.point( 4, 2, 2 );//right eye
player.line( 1, 4, 5, 4, 2);//chin

player.point( 3, 5, 2 );//neck

player.line( 0, 6, 6, 6, 2 );//arms
player.point( 0, 7, 2 ); //left hand
player.point( 3, 7, 2 ); //torso
player.point( 6, 7, 2 ); //right hand

player.point( 2, 8, 2 ); //left knee
player.point( 4, 8, 2 ); //right knee

player.line( 0, 9, 1, 9, 2 );//left foot
player.line( 5, 9, 6, 9, 2 );//right foot
}

//draw some sticks
void draw_sticks( void )
{
sticks.line( 0, 0, 4, 6, 2 );
sticks.point( 5, 2, 2 );
sticks.line( 4, 2, 4, 4, 2 );
}

//randomly place sticks on screen
void place_sticks( void )
{  
  sticks.x = random( GAME_WIDTH  - STICKS_WIDTH  );
  sticks.y = random( GAME_HEIGHT - STICKS_HEIGHT );
}

//at the end of this, player.x/player.y is up to date
void move_player( void )
{
  //Don't have a controller yet...
  if( player.x + dx < 0 || player.x + dx > GAME_WIDTH  - PLAYER_WIDTH  )dx *= -1;
  if( player.y + dy < 0 || player.y + dy > GAME_HEIGHT - PLAYER_HEIGHT )dy *= -1;
  player.x += dx;
  player.y += dy;
}

//Are player and sticks overlapping?
bool collision( void )
{
return abs( player.x - (int)sticks.x ) < ( PLAYER_WIDTH  / 2 + STICKS_WIDTH  / 2 ) &&
       abs( player.y - (int)sticks.y ) < ( PLAYER_HEIGHT / 2 + STICKS_HEIGHT / 2 );
}

//print score in top left
void print_score( void )
{
if( score != MAX_SCORE )
  {
  unsigned char temp_score;
  unsigned char i;
  temp_score = score;
  for( i = 3; i >= 1; i-- )
    {
    TV.print_char((i-1)<<3,0,( temp_score % 10 ) + '0');
    temp_score /= 10;
    }
  }
else
  {
  unsigned char i;
  static const char win[]={'W','I','N'};
  for( i = 0; i <= 2; i++ )
    {
    TV.print_char(i<<3,0,win[i] );
    }
  }
}

void setup()
{
  randomSeed(analogRead(0));
  place_sticks();
  player.x = ( GAME_WIDTH  + PLAYER_WIDTH  ) / 2;
  player.y = ( GAME_HEIGHT + PLAYER_HEIGHT ) / 2;
  dx = 1;
  dy = 1;
  score = 0;
  TV.begin(NTSC,GAME_WIDTH,GAME_HEIGHT);
  TV.select_font(font6x8);
}

void loop()
{
  TV.delay_frame(1);

  move_player();
  TV.clear_screen();
  if( collision() )
      {
        if( score < MAX_SCORE )score++;
        place_sticks();
      }
  print_score();
  draw_player();
  draw_sticks();
}
