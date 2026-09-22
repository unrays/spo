// Copyright (c) September 2026 Félix-Olivier Dumas. All rights reserved.
// Licensed under the terms described in the LICENSE file

// Reference example of the SPO pattern (explicit form)

namespace exotic {

template<template<typename> typename Target, typename Source>
[[nodiscard]] constexpr auto& view_cast(Source& src) noexcept {
    return reinterpret_cast<Target<Source>&>(src);
}

template<
    template<typename> typename Target,
    template<typename> typename Previous,
    typename Source
>
[[nodiscard]] constexpr auto& view_cast(Previous<Source>& src) noexcept {
    return reinterpret_cast<Target<Source>&>(src);
}
    
}

/*******************************************************************/

template<typename Source>
struct RenderView : private Source {
    void draw() { Source::draw(); }
    double area() { return Source::area(); }
};

template<typename Source>
struct EditorView : private Source {
    void draw() { Source::draw(); }
    double area() { return Source::area(); }
    void logDetails() { Source::logDetails(); }
};

/*******************************************************************/

template<typename Source>
void render_scene(RenderView<Source>& shape) {
    shape.draw();
}

template<typename Source>
void inspect_shape(EditorView<Source>& shape) {
    shape.logDetails();
    std::cout << "Computed Area: " << shape.area() << "\n";
    shape.draw();
}

/*******************************************************************/

struct Rectangle {
    double width = 10.0;
    double height = 5.0;

    void draw() const {
        std::cout << "Drawing Rectangle [W: " << width << ", H: " << height << "]\n";
    }

    double area() const {
        return width * height;
    }

    void logDetails() const {
        std::cout << "[Log] Rectangle data loaded.\n";
    }
};

struct Circle {
    double radius = 3.0;

    void draw() const {
        std::cout << "Drawing Circle [Radius: " << radius << "]\n";
    }

    double area() const {
        return 3.14159 * radius * radius;
    }

    void logDetails() const {
        std::cout << "[Log] Circle data loaded.\n";
    }
};

/*******************************************************************/

int main() {
    std::cout << "=== Production Mode (RenderView) ===\n";
    RenderView<Rectangle> rectView;
    RenderView<Circle> circleView;

    render_scene(rectView);
    render_scene(circleView);

    std::cout << "\n=== Editor Mode (EditorView) ===\n";
    EditorView<Rectangle> editorRect;
    EditorView<Circle> editorCircle;

    inspect_shape(editorRect);
    inspect_shape(editorCircle);

    // there is also the unsafe experimental method
    // for more details, I explain the ins and outs in my article on the subject.

    Rectangle rect;
    Circle circle;

    std::cout << "=== Production Mode (RenderView) ===\n";
    render_scene(exotic::view_cast<RenderView>(rect));
    render_scene(exotic::view_cast<RenderView>(circle));

    std::cout << "\n=== Editor Mode (EditorView) ===\n";
    inspect_shape(exotic::view_cast<EditorView>(rect));
    inspect_shape(exotic::view_cast<EditorView>(circle));

    // also possible with this technique, but still really borderline
    // might be interesting for further research and experimentation

    auto& renderRect0 = exotic::view_cast<RenderView>(rect);
    auto& editorRect0 = exotic::view_cast<EditorView>(renderRect0);
}
