/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchapard <lchapard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 14:13:45 by lchapard          #+#    #+#             */
/*   Updated: 2024/07/08 14:13:47 by lchapard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static unsigned int	count_quotes_split(const char *s, unsigned int start,
								unsigned int end)
{
	unsigned int	count;
	int				quotes;

	count = 0;
	quotes = 0;
	while (start <= end)
	{
		if ((s[start] == '\'' || s[start] == '"') && quotes == 0)
		{
			if (s[start] == '\'')
				quotes--;
			else
				quotes++;
			count++;
		}
		else if (((s[start] == '\'' && quotes == -1)
				|| (s[start] == '"' && quotes == 1)))
		{
			quotes = 0;
			count++;
		}
		start++;
	}
	return (count);
}

static unsigned int	count_word(const char *s, const char *charset)
{
	unsigned int	count;
	unsigned int	i;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] && (char_sym(s[i], charset) || ft_isspace(s[i])))
			i++;
		if (s[i] == '\'' || s[i] == '"')
			i++;
		if (s[i])
			count++;
		while (s[i] && (is_in_quotes(s, (int)i) || !ft_isspace(s[i])
				|| !char_sym(s[i], charset)))
			i++;
	}
	return (count);
}

static unsigned int	move_to_current_word(const char *s, const char *set,
								unsigned int current_word, unsigned int *start)
{
	unsigned int	i;

	i = 0;
	while (current_word--)
	{
		while (s[i] && (char_sym(s[i], set) || ft_isspace(s[i])))
			i++;
		*start = i;
		while (s[i] && (is_in_quotes(s, (int)i) || !ft_isspace(s[i])
				|| !char_sym(s[i], set)))
			i++;
	}
	return (i);
}

char	*alloc(const char *s, const char *set, unsigned int current_word)
{
	char			*word;
	unsigned int	start;
	unsigned int	end;

	end = move_to_current_word(s, set, current_word, &start);
	word = (char *)malloc(sizeof(char)
			* (end - start + 1 - count_quotes_split(s, start, end)));
	if (!word)
		return (NULL);
	copy_without_quotes(word, s, start, end);
	return (word);
}

char	**split_arguments(const char *s, char *set)
{
	char				**array;
	unsigned int		nb_word;
	unsigned int		current_word;

	current_word = 0;
	nb_word = count_word(s, set);
	array = (char **)malloc(sizeof(char *) * (nb_word + 1));
	if (array == NULL)
		return (NULL);
	while (++current_word <= nb_word)
	{
		array[current_word - 1] = alloc(s, set, current_word);
		if (array[current_word - 1] == NULL)
		{
			while (current_word--)
				ft_free(array[current_word]);
			ft_free(array);
			return (NULL);
		}
	}
	array[current_word - 1] = NULL;
	return (array);
}
