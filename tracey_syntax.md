# Syntax for .tracey scene description files

### Overview
The file format looks sort of like this:
``` JS
// Comment
OBJECT_NAME {
    PARAMETER_A {x, y, z}
    PARAMETER_B {w}
    MATERIAL {
        PARAMETER_C {r, g, b}
        PARAMETER_D {"path/to/texture"}
    }
}
```

### Comments
You can comment out code using `//` for a single line or `/**/` for blocks (just like in C).

### Objects
Objects are declared in the global scope. All object names are followed by a scope enclosed in curly-braces `{}`. In this scope, you can optionally specify parameters. Here are all objects and the parameters they take.  

##### `FLOOR`
An infinite floor plane.  
+ `HEIGHT` (1)
+ `MATERIAL`

##### `SPHERE`
A sphere.  
+ `POSITION` (3)
+ `RADIUS` (1)
+ `MATERIAL`

##### `POINT_LIGHT`
A spherical light source.  
+ `POSITION` (3)
+ `RADIUS` (1)
+ `COLOR` (3)
+ `INTENSITY` (1)

##### `SUN_LIGHT`
A directional light source.  
+ `DIRECTION` (3)
+ `ANGLE` (1)
+ `COLOR` (3)
+ `INTENSITY` (1)

##### `EXTERNAL`
A scene file. The `SOURCE` field is mandatory and should be a quotation-mark-enclosed string `""` which is a relative path to a 3d scene file.
+ `SOURCE` !
+ `POSITION` (3)
+ `ROTATION` (3)
+ `SCALE` (1)

##### `ENVIRONMENT`
An HDRI texture (only .hdr is supported) or a color. There can only be one in a scene. If `SOURCE` is not specified, the `COLOR` field is used.
+ `SOURCE`
+ `COLOR` (3)
+ `ROTATION` (1)

### Parameters
Parameters are declared in an object scope. All parameter names are followed by a scope enclosed in curly-braces `{}`. In this scope, you must provide a number of values separated by commas indicated by the number in parentheses (unless stated otherwise).  

### Materials
Materials are declared in an object's scope that accepts a material. They also have parameters, like objects do. A parameter can either be a value (like described under *Parameters*) or a texture file path (as described under *Objects/EXTERNAL*).
+ `COLOR` (3)
+ `ROUGHNESS` (1)
+ `METALLIC` (1)
+ `EMISSION` (3)
+ `NORMAL` (3)
+ `TRANSMISSION` (1)
+ `IOR` (1) *index of refraction*
+ `ALPHA` (1) *opacity*
