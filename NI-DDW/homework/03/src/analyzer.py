import numpy as np
import pandas as pd
import networkx as nx
import matplotlib.pyplot as plt

from networkx.drawing.nx_agraph import graphviz_layout
from node2vec import Node2Vec
from sklearn.decomposition import PCA


# Constants
CENTRALITIES = [nx.degree_centrality, nx.closeness_centrality, nx.betweenness_centrality,
                nx.eigenvector_centrality]


def load_graph(filename="casts.csv", row_limit=10000, filter_film_id=None):
    data = pd.read_csv(filename, sep=";").head(row_limit)  # limit to first 1 000 records
    if filter_film_id:  # show only few films for visualizations
        data = data[data['film_id'].str.contains(filter_film_id)]
    graph = nx.Graph()

    for ix, row in data.iterrows():
        graph.add_node(row['actor'])
        same_film_actors = data[(data['film_id'] == row['film_id']) & (data.index < ix)]
        for _, row2 in same_film_actors.iterrows():
            graph.add_edge(row['actor'], row2['actor'])

    return graph


def print_graph_info(graph):
    lenV = len(graph.nodes())
    lenE = len(graph.edges())
    density = round(lenE / ((lenV * lenV) / 2) * 100, 2)
    components = nx.components.number_connected_components(graph)

    print(f"Basic information:\n"
          f"Nodes     : {lenV}\n"
          f"Edges     : {lenE}\n"
          f"Density   : {density} %\n"
          f"Components: {components}\n")


def print_centralities(graph, top_k=10, should_print=True):
    for centrality in CENTRALITIES:
        top_actors = sorted(centrality(graph).items(), key=lambda x: x[1], reverse=True)
        for actor, value in top_actors:
            graph.nodes[actor][centrality.__name__] = value
        if should_print:
            print(f"Top 10 actors ({centrality.__name__}):\n")
            for actor, c in top_actors[:top_k]:
                print(f"{actor}: {round(c, 5)}")
            print("")


def print_communities(graph, k=3, top_k=10):
    communities = [nx.community.louvain_communities, nx.community.k_clique_communities]
    for community in communities:
        print(f"Top communities using {community.__name__}:\n")
        generator = community(graph, k)
        top_communities = sorted(generator, key=lambda x: len(x), reverse=True)
        for com in top_communities[:top_k]:
            print(f"{len(com)}: {list(com)}")
        print("")


def dist(graph, node1, node2):
    try:
        return nx.shortest_path_length(graph, node1, node2)
    except nx.NetworkXNoPath:
        return None


def print_distances(graph, user, top_k=10, should_print=True):
    distances = filter(lambda x: not (x[1] is None), [(node, dist(graph, node, user)) for node in graph.nodes()])
    sorted_distances = sorted(distances, key=lambda x: x[1])

    if should_print:
        print(f"Average distance from {user}: {round(np.average([x[1] for x in sorted_distances]), 2)}")
        print(f"Lowest distances from {user}:\n")
        for top_actor in sorted_distances[:top_k]:
            print(f"{top_actor[0]} to {user}: {top_actor[1]}")
        print(f"\nHighest distances from {user}:\n")
        for top_actor in sorted_distances[-top_k:]:
            print(f"{top_actor[0]} to {user}: {top_actor[1]}")

    for actor in sorted_distances:
        graph.nodes[actor[0]]['distance'] = actor[1]


def draw_components(graph, filename='components', show=True):
    plt.rcParams["figure.figsize"] = [19.2, 10.8]
    plt.rcParams["figure.autolayout"] = True
    plt.clf()
    pos = graphviz_layout(graph, prog="fdp")
    nx.draw(graph, pos, with_labels=False, node_size=10)
    plt.savefig(f"{filename}.png")
    if show:
        plt.show()


def draw_centralities(graph, filename='centralities', show=True):
    for centrality in CENTRALITIES:
        plt.clf()
        plt.title(centrality.__name__)
        pos = graphviz_layout(graph, prog="fdp")
        nx.draw(graph, pos, labels={v: str(v) for v in graph}, cmap=plt.get_cmap("bwr"),
                node_color=[centrality(graph)[k] for k in centrality(graph)])
        plt.savefig(f"{filename}-{centrality.__name__}.png")
        if show:
            plt.show()


def draw_communities(graph, filename='communities', show=True):
    communities = {node: cid + 1 for cid, community in
                   enumerate(nx.algorithms.community.k_clique_communities(graph, 3))
                   for node in community}
    for actor, cid in communities.items():
        graph.nodes[actor]['cid'] = cid

    plt.clf()
    pos = graphviz_layout(graph, prog="fdp")
    nx.draw(graph, pos, labels={v: str(v) for v in graph}, cmap=plt.get_cmap("rainbow"),
        node_color=[communities[v] if v in communities else 0 for v in graph])
    plt.savefig(f"{filename}.png")
    if show:
        plt.show()


def draw_embeddings(graph, filename='embeddings', show=True):
    # node2vec
    node2vec = Node2Vec(graph, dimensions=50, walk_length=15, num_walks=200, workers=4)
    model = node2vec.fit(window=5, min_count=1, batch_words=4)

    # visualize
    pca = PCA(n_components=2)
    pca_comps = pca.fit_transform(model.wv.vectors)
    plt.figure(figsize=(19.2, 10.8))
    plt.scatter(pca_comps[:, 0], pca_comps[:, 1], c='blue')
    for name, x, y in zip(graph.nodes(), pca_comps[:, 0], pca_comps[:, 1]):
        plt.annotate(name, xy=(x, y), size=15)
    plt.savefig(f"{filename}.png")
    if show:
        plt.show()


if __name__ == '__main__':
    G = load_graph()
    print_graph_info(G)
    print_centralities(G)
    print_communities(G)
    print_distances(G, 'Leslie Howard')

    G2 = load_graph(row_limit=50000, filter_film_id='58')
    draw_components(G2)
    print_centralities(G2, should_print=False)
    draw_centralities(G2)
    print_distances(G2, 'Steve Reeves', should_print=False)
    draw_communities(G2)
    draw_embeddings(G2)
    nx.write_gexf(G2, 'export.gexf')
