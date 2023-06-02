import math
import sys

import numpy as np
import pandas as pd
from surprise import Dataset, SVD, Reader


# Loads movies from dataset and returns the movie – genre vectors
def load_movies():
    movies = pd.read_csv('data/movies.csv')
    genres_split = movies['genres'].str.get_dummies(sep='|')  # split by genres
    movies = pd.concat([movies, genres_split], axis=1)
    return movies.drop(['genres', 'title'], axis=1)


# Perform aggregation for users based on genres
# sum based on ratings for each genre
def custom_agg(row):
    series = {}
    for column in set(row.columns.tolist()) - {'rating', 'userId', 'movieId', 'title'}:
        series[column] = np.sum(row.loc[row[column] == 1, 'rating'])
    return pd.Series(series)


# Load training set for ratings
def load_users(movies):
    ratings = pd.read_csv('data/train-ratings.csv')
    users = pd.merge(ratings, movies, on='movieId')
    users = users.drop(['movieId', 'timestamp'], axis=1)
    users['rating'] = users['rating'].apply(lambda rating: 0 if rating < 2.5 else 1)  # convert
    return users.groupby('userId', group_keys=False).apply(custom_agg)  # map


# Cosine similarity function from tutorial
# compares vectors on common columns
def cosine_sim(v1, v2):
    indexes = np.intersect1d(v1.dropna().index, v2.dropna().index)
    A = np.sum(v1 * v2)
    B = math.sqrt(np.sum((v1 * v1).loc[indexes])) * math.sqrt(np.sum((v2 * v2).loc[indexes]))
    return 0 if B == 0 else A / B  # N/A protection


# Pearson's similarity function from tutorial
# compares vectors on common columns
def pearson_sim(v1, v2):
    indexes = np.intersect1d(v1.dropna().index, v2.dropna().index)
    mv1 = np.mean(v1.loc[v1.dropna().index])
    mv2 = np.mean(v2.loc[v2.dropna().index])
    A = np.sum((v1.loc[indexes] - mv1) * (v2.loc[indexes] - mv2))
    B = math.sqrt(np.sum((v1.loc[indexes] - mv1) * (v1.loc[indexes] - mv1))) * math.sqrt(
        np.sum((v2.loc[indexes] - mv2) * (v2.loc[indexes] - mv2)))
    return 0 if B == 0 else A / B  # N/A protection


# Find most similar items to given user
# returns top k similar items using given similarity function
def similar_items(users, movies, user_id, k=30, fn=cosine_sim):
    user = users.loc[user_id].sort_index()
    sim = movies.apply(lambda movie: fn(user, movie), axis=1).sort_values(ascending=False)
    return sim.head(k)


# Finds k nearest neighbours using given similarity function
def knn(data, user_id, k=30, fn=cosine_sim):
    user = data.loc[user_id]
    sim = data.apply(lambda u: fn(u, user), axis=1).sort_values(ascending=False).head(k + 1).index.values
    return np.setdiff1d(sim, user_id)


# Recommends ratings for given data based on rating of top k similar users
# returns top `top` items
def recommendations(data, ix, k=30, fn=cosine_sim):
    sim = knn(data, ix, k, fn)
    cand = data.columns[np.where(data.loc[ix].isna())]
    pred = np.mean(data.loc[sim, cand], axis=0)
    return pred.sort_values(ascending=False)


# Content recommender system for user
# does not recommend items, which were already rated
# k is used for filtering top k similar items
def content_rs(user_id, k=1000, fn=cosine_sim):
    movies = load_movies()
    users = load_users(movies)
    ratings = pd.read_csv('data/train-ratings.csv')

    sim = similar_items(users, movies, user_id, k=k, fn=fn)
    rated = ratings[ratings['userId'] == user_id]['movieId']
    sim = sim.rename(lambda ix: movies.loc[ix]['movieId'])
    return sim.drop(sim[sim.index.to_series().apply(lambda ix: ix in rated)].index)


# Collaborative recommender system
# recommends top `top` items which were found as similarity with top `k` users
def collaborative_rs(user_id, k=1000):
    data = pd.read_csv(f"data/train-ratings.csv")
    ml_data = data.pivot(index="userId", columns="movieId", values="rating")
    recommend = recommendations(ml_data, user_id, k=k)
    return recommend.map(lambda rating: rating / 5.0)  # normalize to [0.0, 5.0]


# Hybrid recommender system
# weights results from content recommender system and collaborative filtering
# with corresponding weights and returns top `top` items after weighting
def hybrid_rs(content, collaborative, content_weight=0.3, collaborative_weight=0.7):
    matching = np.intersect1d(content.index, collaborative.index)
    weighted = pd.Series(matching, index=matching)\
        .map(lambda ix: content.loc[ix] * content_weight + collaborative.loc[ix] * collaborative_weight)
    return weighted.sort_values(ascending=False)


# Recommender system using framework
# returns top `top` items
def framework_rs(user_id):
    # Load data
    data = Dataset.load_from_file('data/train-ratings.csv', reader=Reader(sep=",", skip_lines=1))
    movies = pd.read_csv('data/movies.csv')
    # Train SVD algorithm
    algo = SVD()
    algo.fit(data.build_full_trainset())
    # Calculate top similar items
    sim = movies.apply(lambda movie: algo.estimate(user_id, movie['movieId']), axis=1)
    sim = sim.sort_values(ascending=False).rename(lambda ix: movies.loc[ix]['movieId'])
    return sim.map(lambda rating: rating / 5.0)  # normalize to [0, 5.0]


# Random recommender system
# returns random `top` movies from the dataset
# used for comparison of performance
def random_rs(top=10):
    movies = load_movies()
    random = []
    for x in range(0, top):
        random.append(movies['movieId'].sample().values[0])
    return random


# Calculate precision, recall and F-score
# code from tutorial 05
def calculate_prf(result, reference):
    if len(result) == 0 or len(reference) == 0:
        return 0, 0, 0

    good = 0
    # precision
    for doc in result:
        if doc in reference:
            good += 1
    precision = good / len(result)

    # recall
    good = 0
    for doc in reference:
        if doc in result:
            good += 1
    recall = good / len(reference)

    # f-score
    if precision == 0 or recall == 0:
        return precision, recall, 0

    fs = 2 * ((precision * recall) / (precision + recall + 1))
    return precision, recall, fs


# Load expected ratings from the evaluation file
def load_expected_ratings(user_id, top=10):
    ratings = pd.read_csv('data/test-ratings.csv')
    ratings = ratings[ratings['userId'] == user_id]
    return list(ratings['movieId'].sort_values(ascending=False).head(top))


# Calculate all recommendations for all systems
# and print their respective scores
def compare_rs(user_id, file=sys.stdout, k=10000, top=1000):
    content = content_rs(user_id, k)
    collaborative = collaborative_rs(user_id, k)
    hybrid1 = hybrid_rs(content, collaborative, content_weight=0.3, collaborative_weight=0.7)
    hybrid2 = hybrid_rs(content, collaborative, content_weight=0.5, collaborative_weight=0.5)
    hybrid3 = hybrid_rs(content, collaborative, content_weight=0.7, collaborative_weight=0.3)
    framework = framework_rs(user_id)
    random = pd.Series(index=random_rs(top), dtype=float)

    names = ['content', 'collab ', 'hybrid1', 'hybrid2', 'hybrid3', 'framew ', 'random ']
    expected = load_expected_ratings(user_id, top=top)
    for i, result in enumerate([content, collaborative, hybrid1, hybrid2, hybrid3, framework, random]):
        p, r, f = calculate_prf(result.head(top).index.values, expected)
        print(f"{user_id},{names[i]},{p},{r},{f}", file=file)
    file.flush()


if __name__ == '__main__':
    compare_rs(380)
