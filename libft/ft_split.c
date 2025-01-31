/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 21:06:16 by ilkaddou          #+#    #+#             */
/*   Updated: 2025/01/31 01:06:08 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	num_words(char const *s, char c, int in_dquotes, int in_squotes)
{
	size_t	num;

	num = 0;
	if (!*s)
		return (0);
	while (*s)
	{
		while (*s == c && !in_dquotes && !in_squotes && *s)
			s++;
		if (*s)
		{
			num++;
			while ((*s != c || in_dquotes || in_squotes) && *s)
			{
				if (*s == '"' && !in_squotes)
					in_dquotes = !in_dquotes;
				if (*s == '\'' && !in_dquotes)
					in_squotes = !in_squotes;
				s++;
			}
		}
	}
	return (num);
}

static void	ft_free_split(char **tab)
{
	int	i;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

static size_t	ft_word_len(char const *s, char c)
{
	size_t	word_len;
	int		in_dquotes;
	int		in_squotes;

	word_len = 0;
	in_dquotes = 0;
	in_squotes = 0;
	while (s[word_len] && (s[word_len] != c || in_dquotes || in_squotes))
	{
		if (s[word_len] == '"' && !in_squotes)
			in_dquotes = !in_dquotes;
		if (s[word_len] == '\'' && !in_dquotes)
			in_squotes = !in_squotes;
		word_len++;
	}
	return (word_len);
}

static char	*ft_clean_word(char const *s, size_t len,
			int in_dquotes, int in_squotes)
{
	char	*word;
	size_t	i;
	size_t	j;

	word = (char *)malloc(len + 1);
	if (!word)
		return (NULL);
	i = 0;
	j = 0;
	while (j < len)
	{
		if ((s[j] == '"' && !in_squotes) || (s[j] == '\'' && !in_dquotes))
			;
		else
			word[i++] = s[j];
		if (s[j] == '"' && !in_squotes)
			in_dquotes = !in_dquotes;
		if (s[j] == '\'' && !in_dquotes)
			in_squotes = !in_squotes;
		j++;
	}
	word[i] = '\0';
	return (word);
}

char	**ft_split(char const *s, char c)
{
	char	**res;
	size_t	i;
	size_t	word_len;

	i = 0;
	if (!s)
		return (NULL);
	res = (char **)malloc((num_words(s, c, 0, 0) + 1) * sizeof(char *));
	if (!res)
		return (NULL);
	while (*s)
	{
		while (*s == c && *s)
			s++;
		if (*s)
		{
			word_len = ft_word_len(s, c);
			res[i] = ft_clean_word(s, word_len, 0, 0);
			if (!res[i++])
				return (ft_free_split(res), NULL);
			s += word_len;
		}
	}
	res[i] = NULL;
	return (res);
}
/*int main()  
{  
	char *s = "Bonjour Bonjour ";
    char c = ' ';
	int i;  
    char **result = ft_split(s, c);
	if (result)
	{
		i = 0;
		while (result[i] != NULL)
		{
			printf("%s\n", result[i]);
			free(result[i]);
			i++;
		}
		free(result);
	} 
    return 0;  
}*/
