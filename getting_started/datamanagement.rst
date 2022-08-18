===============
Data management
===============

All the framework is composed of templates that make the development and use of ML (Machine Learning) algorithms easy and allow to the user to select the best available data type for the points of the data features.

One of the main concerns on ufjfmltk is to make ML algorithms development and usage simple, to accomplish that its necessary for the framework to have algorithms that can be applied on the underlying data structures,
one problem with that is that the user needs to get familiar with the available methods which can increase the framework learning curve. To solve that problem, data structures like the Data and Point templates are implemented
trying to keep compatibility with C++ STL library whethever is possible. With that, there's no need to implement a, for example, sorting algorithm when there's already the STL implementation for sorting operations.

In all the examples we'll be using the double data type as default, but you can substitute by any type that makes sense for your problem.

The Point template
===================
This class is a wrapper for the n-dimension variables of the dataset in the feature space, it also includes 
along with the features values **X**, the target function **Y** and the _alpha_ weight associated to each point used in dual versions of ML algorithms.

In this section we'll be learning the basics of a point manipulation and the operations that can be aplied to it, at the end I'll also show
point expressions, a feature that can make the code easier to read and less error prone than the raw usage of loops. 

.. _pointusage:

Initialization
-------------------------

There are several ways to instantiate the Point template, the most simple instatiation is to just initialize the point
with *n* positions filled with zeros. The other properties can be initialized with its own methods.

.. code-block:: cpp

    int n = 3, 
    mltk::Point<double> p(n);

    p.Y() = 1;
    p.Alpha() = 0;
    p.Id() = 5;


A more advanced initialization would be, for example, filling it with random values following a uniform distribution, something 
that can be accomplished calling the ``random_init`` method.

.. code-block:: cpp

    mltk::Point<> p1;

    mltk::random_init(p1, 3, 42);


In the code snipet above we initialized *p1* with 3 dimensions and values following a uniform random distribution with seed 42.
Bellow we can see another example using a different distribution.

.. code-block:: cpp

    mltk::Point<double> p1;

    p1 = mltk::random_init<double, std::fisher_f_distribution<double>>(2.0, 2.0, 3, 42);

Point expressions and operations
--------------------------------

The main advantages of the Point template is the possibility to write code similar to math equations, taking away the burden of writting complex loops
to implement functions. Point expressions are made possible through the implementation of a expression template, it makes possible to overload the math equations
with similar sintax as the paper written version without losing performance when compiler optimizations are turned on (``-O3``). Even on debug mode, the performance loss is acceptable
given that the code is less error prone and easier to read. One example of application is on the implementation of distance metrics like the euclidean distance.

Euclidean distance:

.. math::
    d = \sqrt { \sum_{i=0}^{n-1} (p1_{i} - p2_{i})^{2}}


.. code-block:: cpp

    mltk::Point<> p1(3, 2), p2(3, 1.5);

    double d = std::sqrt(mltk::pow(p1 - p2, 2).sum()); // Euclidean distance between p1 and p2


Another advantage is that the equation is lazy evaluated, i.e only evaluated when it's result is required, like when doing a point assignment. This becomes an advantage on multithreaded environments, when
we want to evaluate an equation when a thread requires it. 

Alongside with the Point template there are some operations that can be applied on the points, they are the basic arithmetic operations and some common math functions.


Examples of arithmetic operations:

.. code-block:: cpp
    
    mltk::Point<> a(3, 1.0), b(3, 2.0);
    mltk::Point<> c = a + b;

    b = a - c;
    a = c / 2;
    c *= 3;


Also were implemented on the framework common math functions that can be aplied directly to the point objects.

.. code-block:: cpp
    
    mltk::Point<> a(3, 1.0), b(3, 2.0);
    mltk::Point<> c = mltk::sin(a) + 2 * mltk::cos(b);
    
    auto d = mltk::pow(mltk::exp(c), 3);
    double sum = d.sum();

This example above only shows a subset of operations that can be applied, for more you can see the list below.

* abs ``mltk::abs(p)`` - absolute values;
* max ``mltk::max(p)`` - maximum value;
* min ``mltk::min(p)`` - minimum value;
* norm ``mltk::norm(p, norm_type)`` - computes the norm of a point, by default ``norm_value = 2``;
* dot ``mltk::dot(p, q)`` - computes the dot product between *p* and *q*;
* log ``mltk::log(p)`` - natural log values; 
* normalize ``mltk::normalize(p, norm_type)`` - normalize a point, by default ``norm_value = 2``;
* linspace ``mltk::linspace(lower, upper, N)`` - returns a point with *N* linear values from *lower* to *upper*. 


We can also print the point content using the stream overload operator.

.. code-block:: cpp
    
    std::cout << p << std::endl;

You can access the features values of a point accessing the elemens of the **x** vector member or by treating the point as a container:

.. code-block:: cpp
    
    int i, dim = p.x.size();

    for(i = 0; i < dim; i++){
        std::cout << p[i] << std::endl;
    }

    // using iterators
    for(auto it = p.begin(); it != p.end(); it++){
        std::cout << (*it) << std::endl;
    }

    
Algorithms
------------

As we are keeping the compatibility with STL, there are several algorithms that are supported by the framework, for example if we want to
fill a Point with integers we can use the ``std::iota`` algorithm for that, like standart C++ containers.

.. code-block:: cpp

    mltk::Point<> p(5);

    std::iota(p.begin(), p.end(), 1);

This code will fill *p* with values ranging from 1 to 5.


We also could initialize a point with random values ranging from 1 to 10 and sort it after.

.. code-block:: cpp

    mltk::Point<int> p(5);

    p = mltk::random_init<int, std::uniform_int_distribution<int>>(1, 10, 5, 42);
    std::sort(p.begin(), p.end());


The Data template
=================
As we're normally dealing with datasets we have multiple points to work, so there's the necessity to have a class to wrap all the information about this dataset and the operations that we can apply to these data. As the 
Point the Data template is also compatible with STL algorithms. 

These are the supported formats to load datasets:

- arff
- csv
- data
- txt (Embrapa datasets format) 

.. _datamemorysharing:

Memory sharing between Data objects
-----------------------------------

Sometimes we need to run several algorithms in the same dataset and, if we'll not transform the feature space of the variables, copying all the data to each algorithm that we'll run can be a waste of memory and at sometimes a simple computer can't handle the memory consumption. 
Thinking in that the Data class was developed using smart pointers, a tool introduced at C++11 that handles the sharing of memory between objects with almost the same speed of raw pointers, but memory safe.

Because of that an array of points in the data class is defined with T as a generic data type as:

.. code-block:: cpp
    
    std::vector<std::shared_ptr<Point< T > > > points;

So if you use the = operator with other data object, they will be point to the same memory space of the original object, to make a deep copy the content of an object to another you'll have to use the ``copy()`` method.

.. code-block:: cpp
    
    Data<double> other;

    other = data.copy()

.. _loadingdataset:

Loading a dataset to a Data object
----------------------------------

This can be easily done with the Data class initialization, accomplished with only one line of code.

.. code-block:: cpp
    
    Data<double> data("wine.csv");


Or if you want the data object initially empty.

.. code-block:: cpp
    
    Data<double> data;

    data.load("wine.csv");

If the target function value or expected value is at the end of the dataset, it must be informed to the constructor.

.. code-block:: cpp
    
    Data<double> data("wine.arff", true);


Note that in all formats the target function must be at the beginning or at the end of each line of the file. You can print 
all the dataset with the C++ standard output stream operator.

.. code-block:: cpp
    
    Data<double> data("wine.csv");

    std::cout << data << std::endl;


.. _datasetinformation:

Getting information about the dataset
-------------------------------------

After the data is loaded into the memory, we can get some useful information about the data.

.. code-block:: cpp
    
    std::cout << "Dataset information: " << std::endl;
    std::cout << "Number of points: " << data.size() << std::endl;
    std::cout << "Dimension: " << data.dim() << std::endl;
    std::cout << "Classes: " << data.classes() << std::endl;
    std::cout << "Classes distribution: " << data.classesDistribution() << std::endl;

.. _scanningpoints:

Scanning through the data points
--------------------------------

There are two ways to access the points contained on a Data object, the first is the operator ``[]`` that returns a smart pointer to a point contained in the Data object, the other
way is through the function call operator ``()`` that returns a reference to the Point object. Almost all the times we would want to use the second option to avoid the pointer sintax. 


Though the smart pointers are intended to be preferred in the place of the raw pointers, they work almost the same way as we are used with the classic pointers, so there's no much difference in this. 

In this example we'll see how we can print each point of the dataset:

.. code-block:: cpp
    
    int i, j, size = data.size(), dim = data.dim();

    for(i = 0; i < size; i++){        
        std::cout << data(i) << std::endl;
    }


Treating the Data object as a container:

.. code-block:: cpp
    
    for(i = 0; i < size; i++){
        for(j = 0; j < dim; j++)
            std::cout << data(i)[j] << std::endl;
    }

Applying transformations to data
---------------------------------

Often we dont want only to load the data but also want to apply transformations to it, be it a point/feature removal or a preprocessing step.
For it the Data template provide methods for point and features removal/insertion and the method ``apply`` that allows to apply a function to 
the points contained in the object.

We could normalize the dataset points like this, instead of looping through the points:

.. code-block:: cpp

    mltk::Data<> data("iris.csv");

    auto normalization = [](mltk::PointPointer<double> point){
        *point = mltk::normalize(*point, 2);
    };

    data.apply(normalization);

Initializing a data object with 10 uniform distributed random points:

.. code-block:: cpp

    mltk::Data<> data;
    for(int i = 0; i < 10; i++){
        auto p  = mltk::random_init(3, 42);

        data.insertPoint(p);
    }

Below are the methods of insertion/removal:

* insertPoint - insert a point to the dataset;
* removePoint - remove a point with the given unique id;
* removeFeature - remove a dimension with the given id (1..dim) from the dataset.

You can see the concepts presented here in practice on the implementation of `algorithms for artificial datasets generation`__.


.. _datasetsimp: https://github.com/mateus558/UFJF-Machine-Learning-Toolkit/blob/main/subprojects/ufjfmltk-core/src/Datasets.cpp
__ datasetsimp_