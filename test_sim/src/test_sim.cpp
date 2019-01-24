#include <ros/ros.h>
#include <visualization_msgs/Marker.h>

#include <cmath>
// 道路参数
double cross_x = 0;// 路口中心x坐标
double cross_y = 0;// 路口中心y坐标
double length = 20;// 路全长
double width = 2;// 路全宽
// 车辆参数
double car_length = 0.7;// 车长
double car_width = 0.7;// 车宽
// 创建路口
void create_cross(visualization_msgs::Marker &line_list, double c_x, double c_y){
    // 参量分别为:图层，路口中心x坐标，路口中心y坐标
    geometry_msgs::Point p;
    // part1
    p.x = c_x + length/2;
    p.y = c_y - width/2;
    p.z = 0;
    line_list.points.push_back(p);
    p.x = c_x + width/2;
    p.y = c_y - width/2;
    p.z = 0;
    line_list.points.push_back(p);
    line_list.points.push_back(p);
    p.x = c_x + width/2;
    p.y = c_y - length/2;
    p.z = 0;
    line_list.points.push_back(p);
    // part2
    p.x = c_x + length/2;
    p.y = c_y + width/2;
    p.z = 0;
    line_list.points.push_back(p);
    p.x = c_x + width/2;
    p.y = c_y + width/2;
    p.z = 0;
    line_list.points.push_back(p);
    line_list.points.push_back(p);
    p.x = c_x + width/2;
    p.y = c_y + length/2;
    p.z = 0;
    line_list.points.push_back(p);
    //part3
    p.x = c_x - length/2;
    p.y = c_y + width/2;
    p.z = 0;
    line_list.points.push_back(p);
    p.x = c_x - width/2;
    p.y = c_y + width/2;
    p.z = 0;
    line_list.points.push_back(p);
    line_list.points.push_back(p);
    p.x = c_x - width/2;
    p.y = c_y + length/2;
    p.z = 0;
    line_list.points.push_back(p);
    //part4
    p.x = c_x - length/2;
    p.y = c_y - width/2;
    p.z = 0;
    line_list.points.push_back(p);
    p.x = c_x - width/2;
    p.y = c_y - width/2;
    p.z = 0;
    line_list.points.push_back(p);
    line_list.points.push_back(p);
    p.x = c_x - width/2;
    p.y = c_y - length/2;
    p.z = 0;
    line_list.points.push_back(p);
}
// 创建车辆
void create_car(visualization_msgs::Marker &line_list, double c_x, double c_y){
    // 参量分别为:图层，测试车x坐标，测试车y坐标
    geometry_msgs::Point p;
    p.x = c_x - car_length/2;
    p.y = c_y - car_width/2;
    p.z = 0;
    line_list.points.push_back(p);
    p.x = c_x - car_length/2;
    p.y = c_y + car_width/2;
    p.z = 0;
    line_list.points.push_back(p);
    line_list.points.push_back(p);
    p.x = c_x + car_length/2;
    p.y = c_y + car_width/2;
    p.z = 0;
    line_list.points.push_back(p);
    line_list.points.push_back(p);
    p.x = c_x + car_length/2;
    p.y = c_y - car_width/2;
    p.z = 0;
    line_list.points.push_back(p);
    line_list.points.push_back(p);
    p.x = c_x - car_length/2;
    p.y = c_y - car_width/2;
    p.z = 0;
    line_list.points.push_back(p);
}

int main( int argc, char** argv ){
    ros::init(argc, argv, "points_and_lines");
    ros::NodeHandle n;
    ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 10);
    ros::Rate r(30);
    double x1 = -5, y1 = 0, x2 = 5, y2 = 0;// 测试用，使用时删除
    while (ros::ok()){
        // 初始化地图图层
        visualization_msgs::Marker map;
        map.header.frame_id = "/my_frame";
        map.header.stamp = ros::Time::now();
        map.ns = "points_and_lines";
        map.action = visualization_msgs::Marker::ADD;
        map.pose.orientation.w = 1.0;
        map.id = 0;// ID
        map.type = visualization_msgs::Marker::LINE_LIST;// 类型
        map.scale.x = 0.1;// 线宽
        map.color.r = 1.0;map.color.g = 1.0;map.color.b = 1.0;map.color.a = 1.0;// 色彩

        // 初始化测试车图层
        visualization_msgs::Marker tester;
        tester.header.frame_id = "/my_frame";
        tester.header.stamp = ros::Time::now();
        tester.ns = "points_and_lines";
        tester.action = visualization_msgs::Marker::ADD;
        tester.pose.orientation.w = 1.0;
        tester.id = 1;// ID
        tester.type = visualization_msgs::Marker::LINE_LIST;// 类型
        tester.scale.x = 0.1;// 线宽
        tester.color.r = 1.0; tester.color.a = 1.0;// 色彩，测试车为红色

        // 初始化测试车图层
        visualization_msgs::Marker tested;
        tested.header.frame_id = "/my_frame";
        tested.header.stamp = ros::Time::now();
        tested.ns = "points_and_lines";
        tested.action = visualization_msgs::Marker::ADD;
        tested.pose.orientation.w = 1.0;
        tested.id = 2;// ID
        tested.type = visualization_msgs::Marker::LINE_LIST;// 类型
        tested.scale.x = 0.1;// 线宽
        tested.color.g = 1.0; tested.color.a = 1.0;// 色彩，被测试车为绿色

        // 创建路口
        create_cross(map, cross_x, cross_y);
	// 创建测试车
	create_car(tester, x1, y1);// 传入x1,y1
	// 创建被测试车
        create_car(tested, x2, y2);// 传入x2,y2
        // 发布话题
        marker_pub.publish(map);
	marker_pub.publish(tester);
	marker_pub.publish(tested);
        r.sleep();
    }
}


