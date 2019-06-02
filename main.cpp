//
//  main.cpp
//  project_2
//
//  Created by Huron on 5/30/19.
//  Copyright © 2019 Huron. All rights reserved.
//Using Grahams's Scan to solve convex hull problem

#include <iostream>
#include <stack>
#include <vector>
#include <cmath>
#include <map>

using namespace std;

struct point /* struct representing the points with value x,y */
{
    int x;
    int y;
};

typedef pair<point,float> PAIR;

struct cmp{
    bool operator()(const PAIR& P1,const PAIR& P2){
        return P1.second<P2.second;
    }
};

bool operator<(const point& a, const point& b)
{
    // Usage of std::tie :
    // compares a.x to b.x,
    // then a.y to b.y
    return std::tie(a.x, a.y) < std::tie(b.x, b.y);
}

void PrintSinglePoint(point);
void print_points(vector<point>);
int FindMaxPointByY(vector<point>);
vector<point> SelectionSortPointbyY(vector<point>&);
vector<point> RemoveEqualPoint(vector<point> &);
vector<point> SelectSmallestX(vector<point>&);
vector<point> SelectionSortPointbyX(vector<point> &);
point ChooseBasePoint(vector<point>);
float Distance(point,point);
float CalculateCos(float,float);
void ConstructbyCos(vector<point>,map<point,float>&);
vector<PAIR> OrderByCos(map<point,float>);
vector<point> GetOrderedPoints(vector<PAIR>);
bool CheckCrossProduct(point,point,point);
stack<point> BuildConvexHull(vector<point>,point);
double NormVector(point);

void PrintSinglePoint(point point){
    cout<<"( "<<point.x<<","<<point.y<<" )";
}

void print_points(vector<point> points){
    /* print the vector which cantains points */
    for(int i=0;i<points.size();i++){
        cout<<"( "<<points[i].x<<","<<points[i].y<<" )"<<endl;
    }
}

int FindMaxPointByY(vector<point> points){
    int max=points[0].y;
    int max_index=0;
    for(int i=1;i<points.size();i++){
        if(points[i].y>max){
            max=points[i].y;
            max_index=i;
        }
    }
    return max_index;
}

vector<point> SelectionSortPointbyY(vector<point> &unsorted_points){
    /* sort the unsorted points */
    vector<point> sorted_points;
    while(!unsorted_points.empty()){
        int max_index=FindMaxPointByY(unsorted_points);
        sorted_points.push_back(unsorted_points[max_index]);
        vector<point> ::iterator it;
        it=unsorted_points.begin()+max_index;
        unsorted_points.erase(it);
    }
    reverse(sorted_points.begin(),sorted_points.end());
    return sorted_points;
}


vector<point> RemoveEqualPoint(vector<point> &sorted_points){
    /* make the points set unique */
    int n=static_cast<int>(sorted_points.size());
    for(int i=0;i<n;i++){
        for(int j=i+1;j<n;j++){
            if((sorted_points[i].x==sorted_points[j].x)&&(sorted_points[i].y==sorted_points[j].y)){
                sorted_points.erase(sorted_points.begin()+j);
            }
        }
    }
    return sorted_points;
}

vector<point> SelectSmallestX(vector<point> &sorted_unique_points){
    /* if point1.y==point2.y, we cpmpare their x value and choose the small one as base point. */
    vector<point> finished_points;
    int n=static_cast<int>(sorted_unique_points.size());
    int i;
    int y_value=sorted_unique_points[0].y;
    for(i=1;i<n;i++){
        if(sorted_unique_points[i].y != y_value)
            break;
        finished_points.push_back(sorted_unique_points[i]);
    }
    finished_points.push_back(sorted_unique_points[0]);
    SelectionSortPointbyX(finished_points);
    int m=static_cast<int>(finished_points.size());
    for(int i=m;i<n;i++)
        finished_points.push_back(sorted_unique_points[i]);
    return finished_points;
}

vector<point> SelectionSortPointbyX(vector<point> &points){
    /* similar as sort by y, just use x value to sort the subset of points whose y value are all the smallest in the set. */
    int i,j,min_idx;
    int n=static_cast<int>(points.size());
    for (i=0;i<n;i++){
        min_idx=i;
        for(j=i+1;j<n;j++){
            if(points[j].x<points[i].x)
                min_idx=j;
        }
        swap(points[min_idx],points[i]);
    }
    return points;
}

point ChooseBasePoint(vector<point> points){
    /* very simple function, just select the first element in the vector, but its input must be finished_points who have been preprocessed. */
    point base_point=points[0];
    return base_point;
}

float Distance(point point_x,point point_y){
    /* calculate the distance between two points */
    float x_distance=abs(point_x.x-point_y.x);
    float y_distance=abs(point_x.y-point_y.y);
    float distance_square=pow(x_distance,2)+pow(y_distance,2);
    float distance=sqrt(distance_square);
    return distance;
}

float CalculateCos(float distance_a,float distance_c){
    /* the distance_b directs the x axis, and we define its distance as 1. The function is to calculate the cos value between base point and others to sort other points by cos value */
    float distance_b=1;
    float cos_value;
    cos_value=(pow(distance_a,2)+pow(distance_b, 2)-pow(distance_c,2))/(2*distance_a*distance_b);
    return cos_value;
}


void ConstructbyCos(vector<point> points,map<point,float> &map_cos){
    /* this fuction map points with their cos value, then we will sort them
     e=(1,0) */
    point point_b;
    point_b.x=points[0].x+1;
    point_b.y=points[0].y;
    for (int i=1;i<points.size();i++){
        float distance_a=Distance(points[0], points[i]);
        float distance_c=Distance(point_b, points[i]);
        float cos_value=CalculateCos(distance_a, distance_c);
        map_cos[points[i]]=cos_value;
    }
}

vector<PAIR> OrderByCos(map<point,float> map_cos){
    map<point,float>::iterator iter;
    vector<PAIR> ordered_by_cos_points;
    for(iter=map_cos.begin();iter != map_cos.end();iter++)
        ordered_by_cos_points.push_back(*iter);
    sort(ordered_by_cos_points.begin(),ordered_by_cos_points.end(),cmp());
    for (int i=0;i<ordered_by_cos_points.size();i++)
        cout<<ordered_by_cos_points[i].second<<endl;
    return ordered_by_cos_points;
}

vector<point> GetOrderedPoints(vector<PAIR> ordered_by_cos_points){
    /* ordered_by_cos_points is pair of point and cos value, the function is to get rid of cos value abd exstract points */
    vector<point> orderedpoints;
    for (int i=0;i<ordered_by_cos_points.size();i++){
        point temp_point=ordered_by_cos_points[i].first;
        orderedpoints.push_back(temp_point);
    }
    reverse(orderedpoints.begin(),orderedpoints.end());
    return orderedpoints;
}

double NormVector(point point){
    return sqrt(pow(point.x,2)+pow(point.y,2));
}

double getAngelOfTwoVector(point &pt1, point &pt2, point &c)
{
    double CV_PI =3.1415926535897;
    double theta = atan2(pt1.x - c.x, pt1.y - c.y) - atan2(pt2.x - c.x, pt2.y - c.y);
    if (theta > CV_PI)
        theta -= 2 * CV_PI;
    if (theta < -CV_PI)
        theta += 2 * CV_PI;
    
    theta = theta * 180.0 / CV_PI;
    return theta;
}

bool CheckCrossProduct(point point_1,point point_2,point point_3){
    point vec1;
    point vec2;
    vec1.x=point_2.x-point_1.x;
    vec1.y=point_2.y-point_1.y;
    vec2.x=point_3.x-point_2.x;
    vec2.y=point_3.y-point_2.y;
    double cross_product=(point_2.x-point_1.x)*(point_3.y-point_1.y)-(point_2.y-point_1.y)*(point_3.x-point_1.x);
    if(cross_product>=0){
        cout<<"cross_product= "<<cross_product<<endl;
        cout<<"true"<<endl;
        return true;
    }
    cout<<"cross_product= "<<cross_product<<endl;
    cout<<"false"<<endl;
    return false;
}

stack<point> BuildConvexHull(vector<point> points,point base_point){
    stack<point> s;
    s.push(base_point);
    s.push(points[0]);
    point point_1;
    point point_2;
    int i=1;
    while(!s.empty() && i<points.size()){
        point_2=s.top();
        s.pop();
        point_1=s.top();
        s.pop();
        while(!CheckCrossProduct(point_1, point_2, points[i])){
            point_2=point_1;
            point_1=s.top();
            s.pop();
        }
        s.push(point_1);
        s.push(point_2);
        s.push(points[i]);
        i+=1;
    }
    return s;
}

int main(int argc, const char * argv[]) {
    point point_1;
    point_1.x=1;
    point_1.y=1;
    point point_2;
    point_2.x=3;
    point_2.y=1;
    point point_3;
    point_3.x=2;
    point_3.y=3;
    point point_4;
    point_4.x=5;
    point_4.y=1;
    point point_5;
    point_5.x=1;
    point_5.y=3;
    point point_6;
    point_6.x=6;
    point_6.y=5;
   /* point point_5;
    point_5.x=19;
    point_5.y=3; */
    point base_point;
    vector<point> points;
    vector<point> finished_points;
    points.push_back(point_1);
    points.push_back(point_2);
    points.push_back(point_3);
    points.push_back(point_4);
    points.push_back(point_5);
    points.push_back(point_6);
    print_points(points);
    cout<<endl;
    vector<point> sorted_points=SelectionSortPointbyY(points);
    print_points(sorted_points);
    RemoveEqualPoint(sorted_points);
    finished_points=SelectSmallestX(sorted_points);
    cout<<endl;
    print_points(finished_points);
    cout<<endl;
    base_point=ChooseBasePoint(finished_points);
    PrintSinglePoint(base_point);
    cout<<endl<<endl;
    map<point,float> map_cos;
    ConstructbyCos(finished_points, map_cos);
    for(auto &v: map_cos)
        cout<<v.second<<endl;
    cout<<endl;
    vector<PAIR> ordered_by_cos_points=OrderByCos(map_cos);
    vector<point> orderedpoints=GetOrderedPoints(ordered_by_cos_points);
    cout<<endl;
    print_points(orderedpoints);
    point point1=base_point;
    point point2;
    point point3;
    cout<<"it is the begin"<<endl;
    for(int i=1;i<orderedpoints.size()-1;i++){
        point2=orderedpoints[i];
        point3=orderedpoints[i+1];
        PrintSinglePoint(point1);
        PrintSinglePoint(point2);
        PrintSinglePoint(point3);
        CheckCrossProduct(point1, point2, point3);
        point1=orderedpoints[i];
    }
    bool result=CheckCrossProduct(base_point, orderedpoints[0], orderedpoints[1]);
    PrintSinglePoint(base_point);
    PrintSinglePoint(orderedpoints[0]);
    PrintSinglePoint(orderedpoints[1]);
    cout<<endl;
    stack<point> s=BuildConvexHull(orderedpoints, base_point);
    cout<<endl;
    while(!s.empty()){
        point p=s.top();
        cout<<"("<<p.x<<","<<p.y<<")";
        s.pop();
    }
    return 0;
}

