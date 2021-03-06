#ifndef PRBT_BRIDGE_H
#define PRBT_BRIDGE_H

#include "ros/ros.h"
#include "std_msgs/Int32.h"
#include "std_msgs/Bool.h"
#include "trajectory_msgs/JointTrajectory.h"
#include "control_msgs/JointTrajectoryControllerState.h"
#include "robot_bridge/planned_trajectory_msg.h"

#include "control_msgs/FollowJointTrajectoryActionGoal.h"
#include "control_msgs/FollowJointTrajectoryAction.h"
#include "actionlib/client/simple_action_client.h"
#include "robot_bridge/OperationModes.h"
#include "pilz_msgs/GetSpeedOverride.h"

#include "prbt_hardware_support/BrakeTest.h"
#include "std_srvs/Trigger.h"
#include "std_srvs/SetBool.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <math.h>
#include <vector>


class prbt_bridge {

    public:

        prbt_bridge();
        
        void spinner (void); 

    private:

        ros::NodeHandle nh;

        ros::Publisher manipulator_joint_trajectory_controller_command_publisher; // OLD PUBLISHER TOPIC
        ros::Publisher trajectory_counter_publisher, operation_mode_publisher;
        ros::Publisher current_state_position_publisher, prbt_position_reached_publisher;

        ros::Subscriber trajectory_subscriber, dynamic_trajectory_subscriber, single_point_trajectory_subscriber, current_position_subscriber;

        trajectory_msgs::JointTrajectory planned_trajectory, next_point;
        control_msgs::JointTrajectoryControllerState current_position;

        actionlib::SimpleActionClient<control_msgs::FollowJointTrajectoryAction> *trajectory_client;
		control_msgs::FollowJointTrajectoryActionGoal trajectory_action;
		control_msgs::FollowJointTrajectoryGoal trajectory_goal;

        ros::ServiceClient prbt_unhold_client, prbt_hold_client, get_speed_override_client, prbt_monitor_cartesian_speed_client;
        std_srvs::Trigger prbt_unhold_srv, prbt_hold_srv;
        std_srvs::SetBool prbt_monitor_cartesian_speed_srv;
        pilz_msgs::GetSpeedOverride get_speed_override_srv;

        std_msgs::Bool position_reached;

        ros::Time last_message;

        bool simulation = false;
        bool dynamic_planning = false, static_planning = false, single_planning = false;
        bool first_single_planning = true;
        bool new_static_trajectory_received = false, new_dynamic_trajectory_received = false, new_single_point_trajectory_received = false;

        int trajectory_counter = 0;
        float tolerance = 0, sampling_time = 0, position_error = 0;

        void Planned_Trajectory_Callback (const trajectory_msgs::JointTrajectory::ConstPtr &);
        void Current_Position_Callback (const control_msgs::JointTrajectoryControllerState::ConstPtr &);
        void Dynamic_Trajectory_Callback (const trajectory_msgs::JointTrajectory::ConstPtr &);
        void Single_Point_Trajectory_Callback (const trajectory_msgs::JointTrajectory::ConstPtr &);


        void Compute_Tolerance(trajectory_msgs::JointTrajectory planned_trajectory);
        float Compute_Position_Error (trajectory_msgs::JointTrajectory point);
        void Wait_For_Desired_Position (bool dynamic);

        void Check_Joint_Limits (trajectory_msgs::JointTrajectory *point);
        void Next_Goal (trajectory_msgs::JointTrajectory planned_trajectory, int counter);

        void Publish_Next_Goal (trajectory_msgs::JointTrajectory goal);
        void Publish_Trajectory_Counter (int counter);
        
        
};

#endif /* PRBT_BRIDGE_H */