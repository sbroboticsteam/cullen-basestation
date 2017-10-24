#include "ros/ros.h"
#include <stdlib.h>
#include <sstream>


/**
 * Make sure to include the standard types for any messages you send and any
 * custom types that you define in .msg files (the .h files should be auto
 * build from the .msg files)
 */
#include <std_msgs/String.h>
#include <std_msgs/Int8.h>
#include "tony/dummy.h" // see beginner_tutorials/msg/coord.msg
#include "tony/raw_gps.h" // see beginner_tutorials/msg/coord.msg

/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */
using namespace tony;
int main(int argc, char **argv)
{
  /**
   * The ros::init() function needs to see argc and argv so that it can perform
   * any ROS arguments and name remapping that were provided at the command line.
   * For programmatic remappings you can use a different version of init() which takes
   * remappings directly, but for most command-line programs, passing argc and argv is
   * the easiest way to do it.  The third argument to init() is the name of the node.
   *
   * You must call one of the versions of ros::init() before using any other
   * part of the ROS system.
   */
  ros::init(argc, argv, "talker");

  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
  ros::NodeHandle n;

  /**
   * The advertise() function is how you tell ROS that you want to
   * publish on a given topic name. This invokes a call to the ROS
   * master node, which keeps a registry of who is publishing and who
   * is subscribing. After this advertise() call is made, the master
   * node will notify anyone who is trying to subscribe to this topic name,
   * and they will in turn negotiate a peer-to-peer connection with this
   * node.  advertise() returns a Publisher object which allows you to
   * publish messages on that topic through a call to publish().  Once
   * all copies of the returned Publisher object are destroyed, the topic
   * will be automatically unadvertised.
   *
   * The second parameter to advertise() is the size of the message queue
   * used for publishing messages.  If messages are published more quickly
   * than we can send them, the number here specifies how many messages to
   * buffer up before throwing some away.
   */

  // "chatter" is the name of the 'channel' of communication. The listener will 
  // be looking for one called "chatter" for the string type for example
  ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);
  // ros::Publisher chatter_pub_int = n.advertise<std_msgs::Int8>("chatter_int", 1000);
  ros::Publisher dummy_pub = n.advertise<dummy>("chatter_dummy", 1000);
  ros::Publisher gps_channel = n.advertise<raw_gps>("gps", 1000);

  ros::Rate loop_rate(10);

  /**
   * A count of how many messages we have sent. This is used to create
   * a unique string for each message.
   */
  int count = 0;
  while (ros::ok())
  {
    /**
     * This is a message object. You stuff it with data, and then publish it.
     */
    std_msgs::String msg; 

    std::stringstream ss;
    ss << "hello world " << count;
    msg.data = ss.str();

    ROS_INFO("%s", msg.data.c_str());


    /**
     * The publish() function is how you send messages. The parameter
     * is the message object. The type of this object must agree with the type
     * given as a template parameter to the advertise<>() call, as was done
     * in the constructor above.
     */
    chatter_pub.publish(msg);

    // // Do the same except generate a random int
    // std_msgs::Int8 msg_int;
    // msg_int.data = std::rand() % 100;
    // chatter_pub_int.publish(msg_int);

    // Do the same except send a set of data with arbitrary types (this must be predefined in a .msg file)
    dummy sample;
    sample.x=50; 
    sample.y=100;
    sample.sized_demo[0] = 53;
    sample.sized_demo[1] = 1312;
    sample.sized_demo[2] = 134;
    sample.no_size_demo.push_back(321);
    sample.no_size_demo.push_back(32);
    sample.no_size_demo.push_back(7899);
    dummy_pub.publish(sample);

    raw_gps gps;
    gps.raw_gps_data.push_back(0);
    gps.raw_gps_data.push_back(50);
    gps.size_gps_data[0];
    // gps.size_gps_data.push_back(50);

    gps_channel.publish(gps);

    // Call this at end of "doing stuff"
    ros::spinOnce();
    loop_rate.sleep();
    ++count;
  }


  return 0;
}