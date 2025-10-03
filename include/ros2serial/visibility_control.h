#ifndef ROS2SERIAL__VISIBILITY_CONTROL_H_
#define ROS2SERIAL__VISIBILITY_CONTROL_H_

// This logic was borrowed (then namespaced) from the examples on the gcc wiki:
//     https://gcc.gnu.org/wiki/Visibility

#if defined _WIN32 || defined __CYGWIN__
  #ifdef __GNUC__
    #define ROS2SERIAL_EXPORT __attribute__ ((dllexport))
    #define ROS2SERIAL_IMPORT __attribute__ ((dllimport))
  #else
    #define ROS2SERIAL_EXPORT __declspec(dllexport)
    #define ROS2SERIAL_IMPORT __declspec(dllimport)
  #endif
  #ifdef ROS2SERIAL_BUILDING_LIBRARY
    #define ROS2SERIAL_PUBLIC ROS2SERIAL_EXPORT
  #else
    #define ROS2SERIAL_PUBLIC ROS2SERIAL_IMPORT
  #endif
  #define ROS2SERIAL_PUBLIC_TYPE ROS2SERIAL_PUBLIC
  #define ROS2SERIAL_LOCAL
#else
  #define ROS2SERIAL_EXPORT __attribute__ ((visibility("default")))
  #define ROS2SERIAL_IMPORT
  #if __GNUC__ >= 4
    #define ROS2SERIAL_PUBLIC __attribute__ ((visibility("default")))
    #define ROS2SERIAL_LOCAL  __attribute__ ((visibility("hidden")))
  #else
    #define ROS2SERIAL_PUBLIC
    #define ROS2SERIAL_LOCAL
  #endif
  #define ROS2SERIAL_PUBLIC_TYPE
#endif

#endif  // ROS2SERIAL__VISIBILITY_CONTROL_H_
