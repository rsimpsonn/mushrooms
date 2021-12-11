To adjust the recursion depth, you can change the member variable m_recursionDepth in scenegraph/RayScene.cpp line 18.

Design decisions: all functionality for shadows/reflections is in my RayScene.cpp file. I implemented the recursive reflections by making my original illuminatePixel recursive. It decrements a depth variable to keep track of the recursive frames.

Known bugs: the isect_test.xml file no longer works like the demo. There's a weird spaceship shadow that gets casted on all the objects. Spent some time trying to fix but couldn't find the issue. Also, did not implement the menu settings.
