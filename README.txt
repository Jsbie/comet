3rdparty (compiled with VS2013 x64):
- Opencv 3.1.0
- VTK 7.0.0.rc2
- Eigen 3.2.7
- JsonCpp 1.6.5

Requirements:
- Cuda 7.5
- Qt 5.5
- Visual 2013 (Win64) (because CUDA did not support 2015)
- Kinect SDK 2.0 v1409
- Debugging Tools for Windows 10 (WinDbg)

Setup:
- open proj/comet.pro with QtCreator
- setup build path to bin folder for all build types (debug, release, profile)
- choose VS2013 x64 compiler
- build
- copy necessary dll's (opencv, tbb etc.)
- run