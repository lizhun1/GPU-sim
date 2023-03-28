# A very simple NVIDIA-PTX simulator

 1. You need to install nvidia driver to get NVCC compiler first.
 2. use` NVCC your_cuda_file -ptx -o file_name.ptx` to compile cuda in ptx.
 3. use `make` to build ptx_sim.
 4. use `ptx_sim your_ptx_file sim_func_name` to sim 
 5. check run_log.txt to debug



# This project is unfinished. There are many bugs to fix. NOW only can simulate 8 bytes data type.
 



