/*
 Copyright (c) 2016-2023 Jacob Gordon. All rights reserved.

 Permission to redistribution and use this software in source and binary forms, with or without modification is hereby granted.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <fudge.h>

void Example2(void) {
    declare_cls(NULL);
    float area = 0;
    AnyClass circle = NULL;
    sm(ShapeFactory, strings("Circle"), args(AnyClass*,&circle), floats(0.0f, 0.0f, 4.0f, 0.0));
    m(circle, get area, args(float*,&area));
    printf("Area: %f\n", area);
    
    AnyClass square = NULL;
    sm(ShapeFactory, strings("Square"), args(AnyClass*,&square), floats(0.0f, 0.0f, 2.0f, 0.0));
    m(square, get area, args(float*,&area));
    printf("Area: %f\n", area);
    
    AnyClass rectangle = NULL;
    sm(ShapeFactory, strings("Rectangle"), args(AnyClass*,&rectangle), floats(0.0f, 0.0f, 5.0f, 2.0));
    m(rectangle, get area, args(float*,&area));
    printf("Area: %f\n", area);

    sm(ShapeFactory, strings("Destroy"), args(AnyClass*,NULL), floats(0.0f, 0.0f, 0.0f, 0.0));
    
}