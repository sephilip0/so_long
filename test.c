#include <mlx.h>
#include <stdlib.h>

#define MALLOC_ERROR	1
#define WIDTH			400
#define HEIGHT			400


int	handle_input(int keysym, t_mlx_data *data)
{
	if (keysym == XK_Escape)
	{
		printf("The %d key (ESC) has been pressed\n\n", keysym);
		mlx_destroy_window(data->mlx_ptr, data->win_ptr);
		mlx_destroy_display(data->mlx_ptr);
		free(data->mlx_ptr)
		exit(1);
	}
	printf("The %d key has been pressed\n\n", keysym);
	return (0);
}

int	main()
{
	void	*mlx_connection;
	void	*mlx_window;

	mlx_connection = mlx_init();
	if (mlx_connection == NULL)
		return (MALLOC_ERROR);
	mlx_window = mlx_new_window(mlx_connection,
								HEIGHT,
								WIDTH,
								"My window");
	mlx_hook()
	if (mlx_window == NULL)
    	{
       		mlx_destroy_display(mlx_connection); // Cleanup MLX connection
        	free(mlx_connection);
        	return (MALLOC_ERROR);
    	}
	mlx_loop(mlx_connection);
	mlx_destroy_window(mlx_connection, mlx_window);
	mlx_destroy_display(mlx_connection);
	free(mlx_connection);
}
