# 3D Software Renderer
A simple 3D software renderer built in C++ from scratch without using OpenGL, DirectX or Vulkan.
Only dependencies are for graphics output and image (texture) loading.

This project is heavily inspired from [Tinyrenderer]. I created this to get a deeper understanding of 3D Graphics and OpenGL pipeline along with solidifying my concepts of C++ programming.

## Features
* 3D Software Rasterization pipeline
* Parallelized renderer (using OpenMP)
* Extendable vertex and fragment shaders using C++ virtual functions
* Tangent space normal texture mapping
* Backface culling
* Perspective correct interpolation in fragment shaders
* Hidden faces removal using Z-buffer
* Implementations of Flat, Gouraud, Phong and Normal map shaders.

### Engine features
* SFML Backend for graphics output
* Camera movement around an axis
* Obj model parser
* Templated vector and matrix math library
* Texture loading (using [stbimage])

# Samples
![Head](https://i.ibb.co/whfRFFb/african-head.png "Head")
![Fire hydrant](https://i.ibb.co/V2N0CVz/fire-hydrant.png "Fire hydrant")
![Leather Shoes](https://i.ibb.co/8X2TXHv/leather-shoes.png "Leather Shoes")

## References
* [Tinyrenderer][tinyrenderer]  
* [Scratchapixel 2.0][scratchapixel]

[tinyrenderer]: https://github.com/ssloy/tinyrenderer/wiki
[scratchapixel]: https://www.scratchapixel.com/index.php
[stbimage]: https://github.com/nothings/stb
