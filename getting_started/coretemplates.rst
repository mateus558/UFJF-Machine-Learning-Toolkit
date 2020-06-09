==============
Core templates
==============

All the framework is composed of templates that make the development and use of ML (Machine Learning) algorithms easy and allow to the user to select the best available data type for the points of the data features.

The user can represent the data in the following data types:

.. code-block:: cpp

    int, int8_t, short int, unsigned int, unsigned short int, long long int;
    float, double, long double;
    char, unsigned char;


In all the examples we'll be using the double data type as default, but you can substitute by one of the above.

The Point template
===================
This class is a wrapper for the n-dimension variables of the dataset in the feature space, it also includes along with the features values **X**, the target function **Y** and the _alpha_ weight associated to each point used in dual versions of ML algorithms.

.. _pointusage:
A simple usage of a point
-------------------------

Let's see a simple Point initialization. First we need to instantiate an object from the Point class.

.. code-block:: cpp
    
    int n = 3, 
    Point<double> p(n, 0.0);

    p.y = 1;
    p.alpha = 0;
    p.id = 5;

With that we initialized a 3 dimensional point with the feature values set as zero, target function equal to one, alpha value zero and id 5.
As we are working in a normed space, there are the p-norm and dot product already associated to the Point class.

.. code-block:: cpp
    
    Point<double> other(n, 1.0);

    p.norm();
    p.dot(other);

We can also print the point content using the stream overload operator.

.. code-block:: cpp
    
    std::cout << p << std::endl;

You can access the features values of a point accessing the elemens of the **x** vector member or by treating the point as a container:

.. code-block:: cpp
    
    int i, dim = p.x.size();

    for(i = 0; i < dim; i++){
    std::cout << p[i] << std::endl;
    }

The Data template
=================
As we're normally dealing with datasets we have multiple points to work, so there's the necessity to have a class to wrap all the information about this dataset and the operations that we can apply to these data. 

These are the supported formats:

- arff
- csv
- data
- txt (Embrapa datasets format) 

.. _datamemorysharing:
Memory sharing between Data objects
-----------------------------------

Sometimes we need to run several algorithms in the same dataset, if we'll not transform the feature space of the variables, copy all the data to each algorithm that we'll run can be a waste of memory and at sometimes a simple computer cant handle the memory consumption. 
Thinking in that the Data class was developed using smart pointers, a tool introduced at C++11 that handles the sharing of memory between objects with almost the same speed of raw pointers, but memory safe.
Because of that an array of points in the data class is defined as, with T as one of allowed data types:

.. code-block:: cpp
    
    std::vector<std::shared_ptr<Point< T > > > points;

So if you use the = operator with other data object, they will be point to the same memory space, to copy the content of an object to another you'll have to use the ```copy()``` method.

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

Note that in all formats the target function must be at the beginning or at the end of each line of the file.

.. _datasetinformation:
Getting information about the dataset
-------------------------------------

After the data is loaded into the memory, we can get some useful information about the data.

.. code-block:: cpp
    
    std::cout << "Dataset information: " << std::endl;
    std::cout << "Number of points" << data.getSize() << std::endl;
    std::cout << "Dimension" << data.getDim() << std::endl;
    std::cout << "Positive points" << data.getNumberPositivePoints() << std::endl;
    std::cout << "Negative points" << data.getNumberNegativePoints() << std::endl;

.. _scanningpoints:
Scanning through the data points
--------------------------------

Though the smart pointers are intended to be preferred in the place of the raw pointers, they work almost the same way as we are used with the classic pointers, so there's no much difference in this. 
In this example we'll see how we can print each point of the dataset:

.. code-block:: cpp
    
    int i, j, size = data.getSize(), dim = data.getDim();

    for(i = 0; i < size; i++){
        std::shared_ptr<Point<double> > p = data.getPoint(i);
        
        std::cout << *p << std::endl;
    }


Treating the Data object as a container:

.. code-block:: cpp
    
    for(i = 0; i < size; i++){
        for(j = 0; j < dim; j++)
            std::cout << (*data[i])[j] << std::endl;
    }

Note that the ``data[i]`` returns a shared pointer to the i-th point of the dataset.
