/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiz-ben <tfiz-ben@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 12:43:16 by tfiz-ben          #+#    #+#             */
/*   Updated: 2025/06/26 13:10:41 by tfiz-ben         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	ft_usleep(int ms)
{
	long	start;
	long	current;

	start = get_time_in_ms();
	while (1)
	{
		current = get_time_in_ms();
		if (current - start >= ms)
			break ;
		usleep(500);
	}
}
