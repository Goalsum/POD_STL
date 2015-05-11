# POD_STL
C++ STL without template code inflation

# Overview
This version of STL tries its best to reduce the template code inflation caused by using STL containers.

We achieve this goal by the following approach: Every interface (public member function) of a template container invokes a corresponding member function of its non-template base class, turning the template type of element to void pointer. And the member function of non-template class implements the actual functionality.

Because all the interfaces of template containers are inline functions, the invocations of interfaces will become invocations passing void pointer type parameters at compile time, this avoids generation of multiple template function instances, resulting in no template code inflation.

For the projects that have strict limits with binary executive file size (e.g. some embeded enviroments), this STL version will be very useful.

# Caution
Because the inner implementions have no information of element's type, they use **memcpy** to copy elements. So make sure that the elment of a container is able to be copied this way. That's why this version is called POD_STL. But actually, you don't have to strictly use POD types as element -- the element can have non-virtual member functions, and can use constructor to initiate its members.

Bacause of this limit, the container of POD_STL cannot be used as another container's element (embeded containers). But the pointer to the container can. for example, you cannot use deque\<list\<int\> \> with POD_STL, but deque\<list\<int\> \*\> is legal.

# Reference
The following documents describe POD_STL in details, in Chinese.

**Design goals and implemention approaches:**

**Usage:**

**Guidlines for developers:**

**Discussion:**
