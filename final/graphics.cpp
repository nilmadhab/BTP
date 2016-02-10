#include<graphics.h>
using namespace std;

int main()
{
    int gd=DETECT, gm;
    initgraph(&gd, &gm,NULL);
    
    outtextxy(150,15, "Some Basic Shapes in Graphics.h");      
    
    line(10,50,500,50);
    outtextxy(501,60,"Line");
        
    circle(150,150,50);
    outtextxy(201,201, "Circle");
        
    rectangle(350,100,500,200);
    outtextxy(505, 211, "Rectangle");   
        
    ellipse(300, 300,0,360, 100,50);
    outtextxy(440, 330, "Ellipse");
 
    
    getch();
    closegraph();
    return 0;
}
