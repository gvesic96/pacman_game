/*PACMAN MPI PARALLEL IMPLEMENTATION*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define MAT_DIM 21

const bool mazeMatrix[21][21] = {
{0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0},
{0,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0},
{0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0},
{0,1,0,1,1,1,0,1,0,1,1,1,1,1,0,1,1,1,1,1,0},
{0,1,0,0,0,0,0,1,0,1,0,0,0,1,0,1,0,1,0,0,0},
{0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0},
{0,0,0,1,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0},
{0,1,1,1,0,1,1,1,0,1,0,1,1,1,0,1,1,1,1,1,0},
{0,1,0,1,0,1,0,1,0,0,0,0,0,1,0,1,0,0,0,0,0},
{0,1,0,1,1,1,0,1,1,1,0,1,0,1,0,1,0,1,1,1,0},
{0,1,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,0,0,1,0},
{1,1,1,1,1,1,0,1,1,1,0,1,0,1,0,1,1,1,1,1,1},
{0,0,0,0,0,1,0,1,0,1,0,1,0,1,0,0,0,0,0,0,0},
{0,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,0},
{0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0},
{0,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,0},
{0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0},
{0,1,0,1,0,1,1,1,1,1,1,1,0,1,1,1,0,1,0,1,0},
{0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0},
{1,1,1,1,1,1,1,1,0,1,0,1,1,1,0,1,1,1,0,1,1},
{0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0}
};


void start_point(int coo[])
{
	//acessing global mazeMatrix
	  int count = 0;
	  //determine packman starting point
	  for(int i=0; i<MAT_DIM; i++)
	  {
	    for(int j=0; j<MAT_DIM; j++)
	    {
		if( ((i==0 || i==20) && 0<=j<=20) || ((j==0 || j==20) && 0<=i<=20))
			{ if(mazeMatrix[i][j]==1) count++; }
	    }
	  }

	  int *x_start;
	  int *y_start;
	  x_start = (int*)malloc(count*sizeof(int));
	  y_start = (int*)malloc(count*sizeof(int));

	  int k=0;
	  for(int i=0; i<MAT_DIM; i++)
	  {
	    for(int j=0; j<MAT_DIM; j++)
	    {
		if( ((i==0 || i==20) && 0<=j<=20) || ((j==0 || j==20) && 0<=i<=20))
		{
		  if(mazeMatrix[i][j]==1)
		  {
			x_start[k]=i;
			y_start[k]=j;
			k++;
		  }
		}
	    }
	  }
	  int it = rand()%count;
	  int start_point_x = x_start[it];
	  int start_point_y = y_start[it];

	  free(x_start);
	  free(y_start);


	coo[0] = start_point_x;
	coo[1] = start_point_y;

}


void PrintMaze(char maze[MAT_DIM][MAT_DIM])
{
	for(int i=0; i<MAT_DIM; i++)
	{
		for(int j=0; j<MAT_DIM; j++)
		{
		  printf("%c", maze[i][j]);
		  if(j==MAT_DIM-1) printf("\n");
		}
	}
}

void refresh_pacman(char maze[MAT_DIM][MAT_DIM])
{
	for(int i=0; i<MAT_DIM; i++)
	{
		for(int j=0; j<MAT_DIM; j++)
		{
		  if(maze[i][j]=='C') maze[i][j]=' ';//place blank space instead of pacman
		}
	}

}

void pacman_move(int coor[], char c)
{
	//pacman_move changes coor[] array if movement is on path of ones in the mazeMatrix
	int row, col;
	row=coor[0];
	col=coor[1];

	switch(c)
	{
	  case 'w' : //DOWN
	  {
	    if(row==0) {row = 20; break;}
	    row--; break;//inverted row numbers in matrix top is 0 bot is 20
	  }
	  case 's' : //UP
	  {
	    if(row==20) {row = 0; break;}
	    row++; break;//inverted row numbers in matrix top is 0 bot is 20
	  }
	  case 'a' : //LEFT
	  {
	    if(col==0) {col = 20; break;}
	    col--; break;
	  }
	  case 'd' : //RIGHT
	  {
	    if(col==20) {col = 0; break;}
	    col++; break;
	  }
	  default: break;//do not move
	}

	bool move_permit=0;
	if(mazeMatrix[row][col]==1) move_permit=1;

	if(move_permit)
	{
	  coor[0]=row;
	  coor[1]=col;
	}

}


int win_lose_play(char maze[MAT_DIM][MAT_DIM], int food, bool p_eaten, int sc)
{
	char win[7] = {'Y','o','u',' ','w','o','n'};
	char lose[9] = {'G','a','m','e',' ','o','v','e','r'};

	int w_l_p = 2;//win_lose_play - 2 is for working
	if(food == 0)
	{
	  w_l_p=1;
	  for(int i=0;i<7;i++)
	  {
		maze[10][7+i]=win[i];
	  }
	  PrintMaze(maze);
	  printf("YOUR SCORE IS: %d\n", sc);
	}


	if(p_eaten == 1)
	{
	  w_l_p=0;
	  for(int i=0;i<9;i++)
	  {
		maze[10][6+i]=lose[i];
	  }
	PrintMaze(maze);
	printf("YOUR SCORE IS: %d\n", sc);
	}


	return w_l_p;// win/lose/play flag, 0,1,2 values
}



bool collision_check(int* buff, int buff_size, int* lpp)
{
	bool res=0;
	int i=2;
	int size = buff_size;
	while(i<size)
	{
	//printf("BUFFER JE: %d\n", buff_size);
	  if((buff[0]==buff[i] && buff[1]==buff[i+1]) || (lpp[0]==buff[i] && lpp[1]==buff[i+1]))//buff01 is pacman, rest are ghosts
	  {
	    res = 1;
	    break;
	  }
	i+=2;
	}

    return res;
}




void ghost_move(int coor[], int* p_direction, int* p_steps)
{
	int dir=*p_direction;
	int st=*p_steps;

	int x = coor[0];
	int y = coor[1];

	if(st!=0)
	{
	  st--;
	  switch(dir)
	  {
	    case 0://down
	    {
		if(y==20){y=0; break;}
		y++;break;//reversed on print
	    }
	    case 1://up
	    {
		if(y==0){y=20; break;}
		y--;break;//reversed on print
	    }
	    case 2://left
	    {
		if(x==0){x=20; break;}
		x--;break;
	    }
	    case 3://right
	    {
		if(x==20){x=0; break;}
		x++;break;
	    }
	    default: break;
	  }
	}

	if(st==0)
	{
	  dir=rand()%4;
	  st=rand()%5;
	}

	if(mazeMatrix[x][y]==1)
	{
	  coor[0]=x;
	  coor[1]=y;
	}
	else
	{
	  dir=rand()%4;
	}

	*p_direction=dir;
	*p_steps=st;
}



int main()
{

	time_t t;
	srand((unsigned) time(&t));

	int coordinates[2]={0, 0};//2 element array for exchanging coordinates
	int csize, prank;
	char local_mat[MAT_DIM][MAT_DIM];
	int score = 0;
	int food=0;
	int flag=2;//0 lose-stop, 1 win-stop, 2 play-work
	bool print_permit = 1;

	//variables for ghost walking
	int direction=0;
	int steps=0;


    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &csize);
    MPI_Comm_rank(MPI_COMM_WORLD, &prank);

    char val_backup[csize-2];//value backup
    int last_p_pos[2];//2 element array for saving pacman position from last iteration

//-----------------------------------------GAME INITIALIZATION---------------------------------------

	if(prank == 0)
	{
	  for(int i=0; i<MAT_DIM; i++)
	  {
		for(int j=0; j<MAT_DIM; j++)
		{
		  if(mazeMatrix[i][j]==0)
		    {local_mat[i][j] = '|';} //CORE 0 creates local matrix of chars so it can work with it
		  else
		  {
		    local_mat[i][j] = '.';
		    food++;//counting food
		  }
		}
	  }
	//created local maze with walls and food on CORE 0


	//GENERATING STARTING POINTS RANDOMLY
	//generating pacman starting point
	start_point(coordinates);
	int pacman_start_x = coordinates[0];
	int pacman_start_y = coordinates[1];
	MPI_Send(coordinates, 2, MPI_INT, 1, 0, MPI_COMM_WORLD);
	last_p_pos[0] = pacman_start_x;
	last_p_pos[1] = pacman_start_y;

	//generating ghosts starting points
	for(int dest=2; dest<csize; dest++)
	{
	  bool same = 1;
	  while(same)//same=1 assume generated coordinates are the same as pacmans
	  {
		start_point(coordinates);
		if(coordinates[0] != pacman_start_x || coordinates[1] != pacman_start_y)
		{
		  same=0;
		  MPI_Send(coordinates, 2, MPI_INT, dest, 0, MPI_COMM_WORLD);
		}
	  }//while
	}//for

    }//initialization of starting points for pacman and ghosts on process 0 and sending to other processes


    MPI_Barrier(MPI_COMM_WORLD);
    if(prank != 0)
    {
	//processes 1 2 3 ... receiving starting coordinates for their characters
	MPI_Recv(coordinates, 2, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }




//------------------------------------------GAME RUNNING--------------------------------------------

    MPI_Barrier(MPI_COMM_WORLD);
    while(1)
    {


	if(prank != 0)
	{
	  MPI_Send(coordinates, 2, MPI_INT, 0, 0, MPI_COMM_WORLD);//send all coordinates to core 0
	}


	//FUNCTIONALITY INTEGRATION - CORE 0
	MPI_Barrier(MPI_COMM_WORLD);
	if(prank == 0)
	{

	  int buff_size = (csize-1)*2;
	  int c_buff[buff_size];//coordinates buffer for receiving coordinates from other cores

	  int offset=0;
	  for(int source=1; source<=(csize-1); source++)
	  {
	    MPI_Recv(&c_buff[offset], 2, MPI_INT, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    offset+=2;
	  }


	  //counting eaten points of food
	  if(local_mat[c_buff[0]][c_buff[1]] == '.')
		{score++; food--;};

	  refresh_pacman(local_mat);//search local_maze and place 'blank space' instead of C
	  local_mat[c_buff[0]][c_buff[1]] = 'C';//place C on received coordinates


	  //backup values on ghosts coordinates
	  offset=2;
	  for(int i=0; i<(csize-2); i++)
	  {
		val_backup[i]=local_mat[c_buff[offset]][c_buff[offset+1]];
		offset+=2;
	  }

	  //placing D on received coordinates for ghost
	  offset=2;
	  for(int i=0; i<(csize-2); i++)
	  {
		local_mat[c_buff[offset]][c_buff[offset+1]] = 'D';
		offset+=2;

		//printf("PLACED GHOST NUMBER :%d ", i);//TESTING TESTING
		//printf("%d %d\n", c_buff[2*i], c_buff[(2*i)+1]);//TESTING TESTING
	  }


	  if(print_permit)
	  {
		printf("Current SCORE is: %d\n", score);
		printf("Remaining FOOD is: %d\n", food);
	  	PrintMaze(local_mat);
	  }

	  offset=2;
	  for(int i=0; i<(csize-2); i++)
	  {
		local_mat[c_buff[offset]][c_buff[offset+1]] = val_backup[i];
		offset+=2;
	  }

	  bool eaten = 0;
	  eaten = collision_check(c_buff, buff_size, last_p_pos);//checking for collision between pacman and ghost
	  last_p_pos[0]=c_buff[0];
	  last_p_pos[1]=c_buff[1];//saving last position of pacman

	  flag = win_lose_play(local_mat, food, eaten, score);

	  //insert W A S D for moving pacman
	  if(print_permit && flag==2)
		printf("Press w/a/s/d + enter : \n");

	  char c='0';
	  if(flag==2)
	  {
	    c = getchar();//getchar samples \n character as well -> solved with print_permit
	    if(c==27){flag=0;}//ESC ASCII = 27, terminate program

	    if(flag==2) MPI_Send(&c, 1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);

	    if(c=='w' || c=='a' || c=='s' || c=='d')
		{print_permit = 1;}
	    else
		{print_permit = 0;}
	  }
	}


	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Bcast(&flag, 1, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);
	if(flag==1 || flag==0)break;//TESTING TESTING TESTING


	//PACMAN CONTROL - CORE 1
	MPI_Barrier(MPI_COMM_WORLD);
	if(prank == 1)
	{
	  char c;
	  MPI_Recv(&c, 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	  pacman_move(coordinates, c);
	}


	//GHOSTS CONTROL - CORE 2 3 ...
	if(prank > 1)
	{
	  ghost_move(coordinates, &direction, &steps);//ghost_move function takes coordinates array that is already on core, received from initialization
	}


	MPI_Barrier(MPI_COMM_WORLD);
    }//while(1)


    MPI_Finalize();


	return 0;

}
