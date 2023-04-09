Plugin to use Ashbaugh potential in HOOMD-blue v3

[HOOMD-blue v3](https://glotzerlab.engin.umich.edu/hoomd-blue/) has changed the API massively compared with v2, which results in the [azplugins](https://github.com/mphowardlab/azplugins) realizing Ashbaugh potential calculation cannot be used in HOOMD-blue v3.  

In that case, this project is committed to solving this problem, inspired by the [azplugins](https://github.com/mphowardlab/azplugins) project and [HOOMD-blue v3 documentation](https://hoomd-blue.readthedocs.io/en/latest/components.html).  

# Usage
JUST replace the *example_plugins/pair_plugin* directory in the HOOMD-blue v3 simulation package with the one in this project, and then compile HOOMD-blue.  

Now you can use Ashbaugh potential in HOOMD-blue v3!  

Hope you enjoy it!   

If you encounter any problem when using it, feel free to contact with me!  
E-mail: xsliu16@fudan.edu.cn
