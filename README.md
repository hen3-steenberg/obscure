# obscure
A 2-D rendering framework.
The name is a wordplay on 2-D render. Which can be taken as "to de-render" of which obscure is a synonym.
This is also indicative that this is the first time I will be attempting to write a rendering framework and everything is obscure to me.

# Functionality
* Draw the hello world triangle
* Draw a triangle list
* Draw a triangle list with index buffers

# TODO
- [ ] Add a template customizable pipeline. To make adding / removing pipelines easier
  - [ ] Move to a "module" library in order for spliting template definitions and implementations.
  - [ ] Templatize the ```pipeline_collection``` structure.
  - [ ] Create pipeline templates for common functionality
    - [ ] Implement a pipeline to draw each basic shape
      - [x] Implement a pipeline to draw triangles
      - [ ] Implement a pipeline to draw rectangles
      - [ ] Implement a pipeline to draw squares
      - [ ] Implement a pipeline to draw circles
    - [ ] Implement pipelines to render text
      - [ ] Implement a pipeline to render static text
      - [ ] Implement a pipeline to render dynamic text
