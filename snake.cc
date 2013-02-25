/**
 * SFML Snake
 * An implementation of SNAKE in C++ using the
 * SFML media library.
 * Kept in one file for simplicity, though the various classes
 * could easily be exported to their own header files.
 * This is rather old code I've kept for posterity and as an SFML
 * example.
 *
 * By Adam Shepley, note updates on February, 2013.
 * Not directly licensed, but licences for C++STD and SFML carry 
 * forward respectively.
 */

#define SFML_STATIC

#define NULL 0

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


#include <iostream>
#include <string>
#include <sstream>
#include <vector>


using std::vector;
using std::stringstream;
using std::cout;
using std::endl;
using std::string;

/*
//function used for multithreading
void ThreadFunction(void* UserData)
{
  //convert our userdata back to its original type

  int *testObject = static_cast<int*>(UserData);
  int test2 = *testObject;
}
*/
bool Running = true;
bool playerKilled = false;

//determines the rate of travel
float timeSpeed = 0.1;
float defaultSpeed = 0.1;


int wrapper = 20;

//we declare our score variables and direction enums
int currentScore = 0;
int highScore = 0;

enum direction { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT };
//determines the distance our block travels.
float speed = 20;


//prototypes
bool xySquareCollision(float firstX, float firstY, float secondX, float secondY, float firstModifier, float secondModifier);


class snakeCubeObject
{
  private:
    
    float xpos;
    float ypos;

    
    float nextX;
    float nextY;
    
    direction nextDirection;
    
    
    
  public:
    
    direction currentDirection;
    int myID;
    snakeCubeObject *nextCube;

    snakeCubeObject(float x = 200, float y = 200, float id = 0, snakeCubeObject* next = NULL)
    {
      xpos = x;
      ypos = y;
      myID = id;
      nextCube = next;
      
      if(nextCube != NULL)
      {
	nextX = nextCube->getXPos();
	nextY = nextCube->getYPos();
	currentDirection = nextCube->currentDirection;
      }
    }
    
    void move(direction dir, direction& currentDir, direction nextDir = DIR_LEFT)
    {
     if(myID == 0)
     {
      switch(dir)
      {
	case DIR_UP:
	  if(ypos < 25)
	    ypos = 675;
	  if(currentDir != DIR_DOWN)
	  {
	    ypos -= speed;
	    currentDir = dir;
	  }
	  else
	  {
	    ypos += speed;
	    if(ypos > 675)
	      ypos = 26;
	    currentDir = DIR_DOWN;
	    dir = DIR_DOWN;
	  }
	  break;
	  
	case DIR_DOWN:
	  if(ypos > 675)
	    ypos = 26;
	  
	  if(currentDir != DIR_UP)
	  {
	    ypos += speed;
	    currentDir = dir;
	  }
	  else
	  {
	    if(ypos < 25)
	      ypos = 675;
	    ypos -= speed;
	    currentDir = DIR_UP;
	    dir = DIR_UP;
	  }
	  break;
	 
	case DIR_LEFT:
	  if(xpos < 25)
	    xpos = 675;
	  if(currentDir != DIR_RIGHT)
	  {
	    xpos -= speed;
	    currentDir = dir;
	  }
	  else
	  {
	    xpos += speed;
	    if(xpos > 675)
	      xpos = 26;
	    currentDir = DIR_RIGHT;
	    dir = DIR_RIGHT;
	  }
	  break;
	  
	case DIR_RIGHT:
	  if(xpos > 675)
	    xpos = 26;
	  if(currentDir != DIR_LEFT)
	  {
	    xpos += speed;
	    currentDir = dir;
	  }
	  else
	  {
	    if(xpos < 25)
	      xpos = 675;
	    xpos -= speed;
	    currentDir = DIR_LEFT;
	    dir = DIR_LEFT;
	  }
	  break;
      }
      currentDirection = currentDir;
  //    cout << "currdir is " << currentDirection << endl;
     }
     
     else
     {
       xpos = nextX;
       ypos = nextY;
       //cout << "next x is " << nextCube->getXPos() << " and next y is " << nextCube->getYPos() << " and next dir is " << nextCube->currentDirection << endl;
    /*   if(xpos == nextX && ypos == nextY)
       {
	 nextCube = nextCube;
	// cout << "asdasdnext x is " << nextCube->getXPos() << " and next y is " << nextCube->getYPos() << " and next dir is " << nextCube->currentDirection << endl;
	// sf::Sleep(3000);
	  currentDirection = nextCube->currentDirection;
	  nextX = nextCube->getXPos();
	  nextY = nextCube->getYPos();
	  cout << "\n\nlolnext\n " << nextX << "\n" << nextY << "\n";
       }*//*
	cout << "CURRENT DIR IS      " << currentDirection << endl;
       switch(currentDirection)
       {
	 case DIR_UP:
	   if(ypos < 0)
 	     ypos = 700;
 	   ypos -= speed;
	   break;
	  
	 case DIR_DOWN:
	   if(ypos > 700)
	     ypos = 1;
	   ypos += speed;
	   break;
	 
	 case DIR_LEFT:
	   if(xpos < 0)
	     xpos = 700;
	   xpos -= speed;
	   break;
	  
	 case DIR_RIGHT:
	   if(xpos > 700)
	     xpos = 1;
	   xpos += speed;
	   break;
       }*/
       
     }
    }
    
    float getNextX()
    {
      return nextX;
    }
    
    float getNextY()
    {
      return nextY;
    }
    
    float getXPos()
    {
      return xpos;
    }
    
    float getYPos()
    {
      return ypos;
    }
    
    void setXPos(int newX)
    {
      xpos = newX;
    }
    
    void setYPos(int newY)
    {
      ypos = newY;
    }
    
    void setNextDir(direction dir)
    {
      nextDirection = dir;
    }
    
    void setNextX(float newX)
    {
      nextX = newX;
    }
    
    void setNextY(float newY)
    {
      nextY = newY;
    }
    
  //  snakeCubeObject operator*(snakeCubeObject & param)
   // {
      
    snakeCubeObject* operator*() { return this; }
};



class playerSnake
{

  public:

    sf::Shape snakeShape;


    playerSnake()
    {
      //setup our sprite
      if (!snakeCubeImage.LoadFromFile("snakeCube.tga"))
      {
          cout << "oh god something's gone wrong kitty! Create an image called snakeCube.tga!\n" << endl;
      }
      snakeCubeSprite.SetImage(snakeCubeImage);
      snakeCubeSprite.SetScale(0.4,0.4);
      
      //initialize our first cube
      snakeCubeObject mainCube(200,200,0);
      cubes.push_back(mainCube);

      currentDir = DIR_LEFT;
    }
    
    void draw(sf::RenderWindow &App)
    {
      for(unsigned int i = 0; i < cubes.size(); i++)
      {
	snakeCubeSprite.SetPosition(cubes.at(i).getXPos(), cubes.at(i).getYPos());
	App.Draw(snakeCubeSprite);
	//cout << "\n\nDraw occured?" << endl;
      }
    }
    
    
    
    //Our main movement function/loop
    void move(direction dir = DIR_LEFT)
    {
      float xHead = cubes.at(0).getXPos();
      float yHead = cubes.at(0).getYPos();
      for(unsigned int i = 0; i < cubes.size(); i++)
      {/*
	if((cubes.size() > 1) &&(i < cubes.size()-1) && (cubes.at(i+1).getNextX() == cubes.at(i+1).getXPos()
				&&  cubes.at(i+1).getNextY() == cubes.at(i+1).getYPos()))
	{
	  std::cerr << "PASSED THE POINT\n\n" << endl;
	  cubes.at(i+1).setNextY(cubes.at(i).getYPos());
	  cubes.at(i+1).setNextX(cubes.at(i).getXPos());
	  cubes.at(i+1).setNextDir(cubes.at(i).currentDirection);
	}*/
	if(i > 0)
	  if(xySquareCollision(xHead, yHead, cubes.at(i).getXPos(), cubes.at(i).getYPos(), 17, 17))
	  {
	    cout << "OH NOOOOOOOOOOOOOOOOOOOOOOOOOO" << endl;
	    playerKilled = true;
	    timeSpeed = defaultSpeed;
	  }
	
	if((cubes.size() > 1) && (i < cubes.size()-1))
	{
	//  std::cerr << "STARTED" << endl;
	  cubes.at(i+1).setNextX(cubes.at(i).getXPos());
	 // std::cerr << "BETWEEN";
	  cubes.at(i+1).setNextY(cubes.at(i).getYPos());
	//  std::cerr <<  "changed vars      " << cubes.size() << endl;
	}	
	cubes.at(i).move(dir, currentDir);
	//std::cerr <<  "changed vars 2" << endl;
      }
    }
    
    
    
    void reset()
    {
      unsigned int x = cubes.size();
      for(unsigned int i = 1; i < x; i++)
	cubes.erase(cubes.begin() + 1);
      cubes.front().setXPos(200);
      cubes.front().setYPos(200);
    
    }
    
    void eat()
    {
      
      float xmod = 0, ymod = 0;
      
      switch(currentDir)
      {
	case DIR_UP:
	  xmod = 0;
	  ymod = 20;
	  break;
	
	case DIR_DOWN:
	  xmod = 0;
	  ymod = -20;
	  break;
	
	case DIR_LEFT:
	  xmod = 20;
	  ymod = 0;
	  break;
	  
	case DIR_RIGHT:
	  xmod = -20;
	  ymod = 0;
	  break;
      }
      
      snakeCubeObject newCube(cubes.at(cubes.size()-1).getXPos() + xmod,cubes.at(cubes.size()-1).getYPos() + ymod, cubes.size(), *cubes.at(cubes.size()-1));
      cubes.push_back(newCube);
   //   cout << "NEW CUBE created" << endl;
    }
    
    void killed()
    {
      reset();
    }
    
    snakeCubeObject& getSnakeHead(int position = 0)
    {
      return cubes.at(position);
    }
    
    sf::Sprite snakeCubeSprite;

  private:

    sf::Image snakeCubeImage;
    direction currentDir;
    vector <snakeCubeObject> cubes;


};

class powerPellet
{
  public:
    
    powerPellet(float startx = 400, float starty = 400)
    {
      
      //setup our sprite
      if (!pelletImage.LoadFromFile("pellet.tga"))
      {
          cout << "oh god something's gone wrong kitty! Create an image called snakeCube.tga!\n" << endl;
      }
      pelletSprite.SetImage(pelletImage);
      pelletSprite.SetScale(1,1);
      xpos = startx;
      ypos = starty;
      pelletSprite.SetPosition(xpos,ypos);
     // pelletShape = sf::Shape::Rectangle(xpos,ypos,xpos+10,ypos+10,sf::Color::White, 2);
    }
    
    void draw(sf::RenderWindow &App)
    {
      pelletSprite.SetPosition(xpos, ypos);
      App.Draw(pelletSprite);
    }
    
    
    
    float getXPos()
    {
      return xpos;
    }
    
    float getYPos()
    {
      return ypos;
    }
    
    void reset()
    {
      xpos = sf::Randomizer::Random(100, 600);
      ypos = sf::Randomizer::Random(100, 600);
    }
    
    sf::Sprite pelletSprite;
    
  private:
    
    float xpos;
    float ypos;
    
    sf::Shape pelletShape;
    sf::Image pelletImage;

    
    
    
    
    
};




bool xySquareCollision(float firstX, float firstY, float secondX, float secondY, float firstModifier, float secondModifier)
{
  if((secondX >= firstX && secondX <= firstX+firstModifier)
    ||
    (firstX >= secondX && firstX <= secondX+secondModifier)
    ||
    (firstX <= secondX && secondX <= firstX + firstModifier)
    )
  {
 //   std::cerr << "In the X    ." << endl;
    if((secondY >= firstY && secondY <= firstY+firstModifier)
      ||
      (firstY >= secondY && firstY <= secondY+secondModifier)
      ||
      (firstY <= secondY && secondY <= firstY + firstModifier)
      )
    {
      return true;
    }
  }
  return false;
  
}


bool collisionTest(playerSnake &player, powerPellet &pellet)
{
  snakeCubeObject & head = player.getSnakeHead();
  float playerX = head.getXPos();
  float playerY = head.getYPos();
  
  float pelletX = pellet.getXPos();
  float pelletY = pellet.getYPos();
  /*
  if((pelletX >= playerX && pelletX <= playerX+20)
    ||
    (playerX >= pelletX && playerX <= pelletX+10)
    ||
    (playerX <= pelletX && pelletX <= playerX + 20)
    )
  {
 //   std::cerr << "In the X    ." << endl;
    if((pelletY >= playerY && pelletY <= playerY+20)
      ||
      (playerY >= pelletY && playerY <= pelletY+10)
      ||
      (playerY <= pelletY && pelletY <= playerY + 20)
      )
    {
 //     cout << "ATE!    ." << endl;
      return true;
    }*/
  if(xySquareCollision(playerX,playerY,pelletX,pelletY,20,10))
  {
    return true;
  }
  return false;
  
}


int main(int argc, char** argv)
{
    int x = 700, y = 700;

    sf::RenderWindow App(sf::VideoMode(x, y, 32), "SFML Simple Snake");
    App.SetFramerateLimit(90);
    App.Create(sf::VideoMode(x, y, 32), "SFML Simple Snake", sf::Style::Resize|sf::Style::Close);


    //we create an event for...handling events
    sf::Event Event;

    //we grab our window's default input events
    const sf::Input& Input = App.GetInput();


    //set up our text strings and such

    sf::String title("Simple Snake", sf::Font::GetDefaultFont(), 30);
    title.Move(250,50);

    sf::String scoreT("Score     High Score", sf::Font::GetDefaultFont(), 20);
    scoreT.Move(220,90);

    sf::String score1("0", sf::Font::GetDefaultFont(), 10);
    score1.Move(220,120);

    sf::String score2("0", sf::Font::GetDefaultFont(), 10);
    score2.Move(320,120);
    

    
    playerSnake * Snake = new playerSnake;
    powerPellet * pellet = new powerPellet;
    
    direction dir;
    sf::Clock clock;
    float time;
//    int loopnum = 1;
    
   // bool eatNow = false;
    while (Running)
    {
	time = clock.GetElapsedTime();
        App.GetEvent(Event);
	string score;
        stringstream out;
        if (Event.Type == sf::Event::Closed)
            Running = false;

        App.Clear(sf::Color(0,0,0));


	if(Input.IsKeyDown(sf::Key::Down))
	  dir = DIR_DOWN;
	else if(Input.IsKeyDown(sf::Key::Up))
	  dir = DIR_UP;
	else if(Input.IsKeyDown(sf::Key::Left))
	  dir = DIR_LEFT;
	else if(Input.IsKeyDown(sf::Key::Right))
	  dir = DIR_RIGHT;
	
	if(playerKilled == true)
	{
	  playerKilled = false;
	  Snake->killed();
	 // delete Snake;
	  playerSnake *Snake = new playerSnake;
	  currentScore = 0;
	}
	
	if(time >= timeSpeed)
	{
	  Snake->move(dir);
	  clock.Reset();
	}
	//cout << "CubeX is " << Snake->getSnakeHead().getXPos() << " and CubeY is " << Snake->getSnakeHead().getYPos() << endl;
	//cout << "PelletX is " << pellet->getXPos() << " and pelletY is " << pellet->getYPos() << endl;
	//if we eat a pellet
	
	if(collisionTest(*Snake, *pellet))
	{
	  pellet->reset();
	  currentScore += 100;
	  
	  Snake->eat();
	  out << currentScore;
	  score = out.str();
	  score1.SetText(score);
	  timeSpeed -= 0.0025;
	}
	
	//Our drawing phase
	
	Snake->draw(App);
	pellet->draw(App);
	
	 
        App.Draw(title);
	App.Draw(scoreT);
	App.Draw(score1);
	App.Draw(score2);


        App.Display();
	if(highScore <= currentScore)
	{
	  highScore = currentScore;
	  out << highScore;
	  score = out.str();
	  score2.SetText(score);
	}
	/*
	std::cerr << "loop # " << loopnum << endl;
	loopnum++;
*/
    }

    delete Snake;

    return EXIT_SUCCESS;
}
