.. UFJF-MLTK documentation master file, created by
   sphinx-quickstart on Mon Jun  8 00:10:01 2020.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

================================
UFJF - Machine Learning Toolkit
================================

UFJF-MLTK is a cross-platform framework written in the C++ language for the development and 
usage of machine learning algorithms, addresses several types of learning problems such as classification, regression, 
clustering, feature selection, and ensemble learning. It aims to provide an always growing 
set of algorithms and tools for machine learning researchers and enthusiasts in its projects.

**API Reference**

You can find the API Reference at our repository `Gihub Pages`__.

.. _apipage: https://mateus558.github.io/UFJF-Machine-Learning-Toolkit/
__ apipage_

**Cite us**

If you use our project in your research, you can cite us by adding the bibtex from the `project paper`__::

   @inproceedings{10.1145/3330204.3330273,
      author = {Marim, Mateus Coutinho and de Oliveira, Alessandreia Marta and Villela, Saulo Moraes},
      title = {UFJF-MLTK: A Framework for Machine Learning Algorithms},
      year = {2019},
      isbn = {9781450372374},
      publisher = {Association for Computing Machinery},
      address = {New York, NY, USA},
      url = {https://doi.org/10.1145/3330204.3330273},
      doi = {10.1145/3330204.3330273},
      booktitle = {Proceedings of the XV Brazilian Symposium on Information Systems},
      articleno = {63},
      numpages = {8},
      keywords = {object-oriented programming, machine learning, Framework},
      location = {Aracaju, Brazil},
      series = {SBSI’19}
   }

.. _paper: https://www.researchgate.net/publication/333406079_UFJF-MLTK_a_framework_for_machine_learning_algorithms
__ paper_

**Authors**

* Mateus Coutinho Marim
* Saulo Moraes Villela
* Alessandreia Oliveira

**Contact**

Feel free to contact me at any time to clear doubts that you could have and if you want to contribute to the development of the framework. You can contact me at my e-mail address mateus.marim@ice.ufjf.br.

.. toctree::
   :maxdepth: 2
   :caption: Table of contents

.. toctree::
   :maxdepth: 2
   :caption: Framework overview
   :hidden:
   
   overview/modules
   overview/architecture

.. toctree::
   :maxdepth: 3
   :caption: Getting Started
   :hidden:
    
   getting_started/installation
   getting_started/datamanagement
   getting_started/classification

.. toctree::
   :maxdepth: 2
   :caption: Contribute
   :hidden:
   
   contribute/extending
   contribute/codeconduct
   license
