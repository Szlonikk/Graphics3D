# Debugging and handling errors

Debugging OpenGL code can be frustrating experience. The OpenGL API is a very low level, and it is easy to make
mistakes.

## Catching errors

Each call to the OpenGL API can result in an error. The error is reported by setting the error flag. The error flag can
be read using the `glGetError` function. The function returns the error flag and clears it. If the error flag is set,
the function returns the error code. If the error flag is not set, the function returns `GL_NO_ERROR`.

Currently, the error flag can be set to one of the following values:
`GL_INVALID_ENUM`, `GL_INVALID_VALUE`, `GL_INVALID_OPERATION`, `GL_INVALID_FRAMEBUFFER_OPERATION`, `GL_OUT_OF_MEMORY`, `GL_STACK_UNDERFLOW`, `GL_STACK_OVERFLOW`.

Unfortunately, those are just symbolic constants, and they are not very helpful. I have provided a function that will
translate the error code to a string. The function is called `error_msg`and is defined in `Application/utils.h` file.
The function takes the error code as an argument and returns the string describing the error.

The typical usage of this function is as follows:

```c++
glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle + 1);
    auto error_code = glGetError();
    if (error_code != GL_NO_ERROR) {
        std::cerr << "Error: " << xe::utils::error_msg(error_code) << "  at " << __FILE__ << ":" << (__LINE__ - 3)
                  << std::endl;
    }
```

Macros `__FILE__` and `__LINE__` are replaced by the preprocessor with the current file name and line number. We need to
subtract three form line number as the error was generated in the line with the `glBindBuffer` call. If
the `glBindBuffer` function is called with wrong  `v_buffer_handle` value, the following error message will be printed:

```text
Error: INVALID OPERATION  at /home/pbialas/Dydaktyka/Graphics3D/Teaching/Graphics3DCode/src/Assignments/00_Triangle/app.cpp:52
``` 

Each function in the OpenGL API has a description of the possible errors that can be generated by the function.
For `glBindBuffer` function the description is as follows:

> Errors
>
>GL_INVALID_ENUM is generated if target is not one of the allowable values.
>
>GL_INVALID_VALUE is generated if buffer is not a name previously returned from a call to glGenBuffers.

This description together with the error message should help you find the source of the error.

The error flag is only reset by the call to the `glGetError` function, so if you check for errors on after several calls
to OpenGL API, you will not know which call generated the error. If more than one call generated an error, you will only
get the error code of the first call. So error checking should be done after __each__ call to the OpenGL API.
This is very tedious and error-prone. To make this easier, I have provided a `OGL_CALL` macro that will check for errors
after each call if you wrap each OpenGL function call with this macro. The macro is defined in `Application/utils.h` and
is used as follows:

```c++
OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle));
```

```text
[2023-10-02 12:08:22.009] [critical] OpenGL error: INVALID OPERATION  glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle) /home/pbialas/Dydaktyka/Graphics3D/Teaching/Graphics3DCode/src/Assignments/00_Triangle/app.cpp:52
```

This macro will abort the program if the error is generated, forcing you to fix the error. This behavior can be changed
by defining

```c++
#define DEBUG_NO_ABORT
```

At the top of the file. In this case, the macro will only print the error message and continue execution.

I __strongly__ recommend using the `OGL_CALL` macro. It will save you a lot of time and frustration. As pointed out
before this macro has to be used consistently. If you forget to wrap a function call with this macro, you may get the
wrong error or not detect an error at all.

## Debugging shaders

Another aspect of the OpenGL API is that it is that part of the code is executed on another device (GPU). OpenGL API does not provide any tools for debugging the code running on the GPU. 

