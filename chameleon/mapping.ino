//float traveled = encoder_distance_traveled();
//
void mapping_traveled(float dist)
{
    float rads = radians(robot_info.angle);

    float _cos = cos( rads );
    float _sin = sin( rads );


    robot_info.x += dist * _cos;
    robot_info.y += dist * _sin;
}

void mapping_rotated(int degrees)
{
    robot_info.angle += degrees;

    while(robot_info.angle < PI)
    {
        robot_info.angle += 360;
    }

    while(robot_info.angle > 360)
    {
        robot_info.angle -= 360;
    }
}
