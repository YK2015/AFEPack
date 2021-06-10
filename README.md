# AFEPack

It is a lightweight version of AFEPack (Adaptive Finite Element Package), 
which is developed by Prof. Ruo Li. AFEPack is a C++ library providing 
very flexible data structure to develop the finite element and handle 
the mesh adaptive. 

Here this AFEPack is decoupled from the package deal.II, only Boost and MPI and
C++11 features are required for the installation. 

Specify the compiler in Makefile.1 and do a "make". So much lightweight, hah ^^



---

Change logs (@yk2015)

#### Remarks: Make some changes to adapt the compiler in Jupyter notebook

1. fix error `dependent using declaration resolved to type without 'typename'`

   ```sh
   In file included from /home/kuang/local/include/AFEPack/../bundled/matrix.h:11:.
   /home/kuang/local/include/AFEPack/../bundled/vector.h:119:17: error: dependent using declaration resolved to type without 'typename'
     using base_t::scalar_t;
   ```

   Fix it in `AFEPack/bundled/vector.h:119`

   ```cpp
    //using base_t::scalar_t; // changes to
    using typename base_t::scalar_t;
   ```

   

2. fix error 

   ```sh
   /home/kuang/local/include/AFEPack/HGeometry.h:669:19: error: default template argument in a class template partial specialization
   template <int DOW=1>
                     ^
   /home/kuang/local/include/AFEPack/HGeometry.h:703:19: error: default template argument in a class template partial specialization
   template <int DOW=2>
                     ^
   /home/kuang/local/include/AFEPack/HGeometry.h:743:19: error: default template argument in a class template partial specialization
   template <int DOW=3>
   ```

   Fix: delete `=1` ,`=2`, `=3`

3. Add new function in `bundled/sparsity_pattern.h`

   ```cpp
   void
   print_gnuplot (std::ostream &out) const
   {
   	for (index_t i=0; i<rows; ++i)
       for (size_t  j=rowstart[i]; j<rowstart[i+1]; ++j)
         if (colnums[j] != invalid_entry)
           // while matrix entries are
           // usually written (i,j),
           // with i vertical and j
           // horizontal, gnuplot output
           // is x-y, that is we have to
           // exchange the order of
           // output
           out << colnums[j] << " " << -static_cast<signed int>(i) << std::endl;
   }
   ```

   



### 
