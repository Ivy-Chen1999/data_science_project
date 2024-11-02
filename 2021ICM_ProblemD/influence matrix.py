import csv
import numpy as np
import networkx as nx 

music_genre = set()
G = nx.DiGraph()  

# def get_total_influence_number(): 
csv_w = csv.writer(open('pl_data.csv', 'w'))

def read_influence_file():
    head = True
    influence_file = csv.reader(open('import_data.csv', 'r'))
    adj_matrix = np.zeros((artist_num, artist_num))

    for data in influence_file:
        if head:
            head = False
            continue

        influencer_id, influencer_name, influencer_main_genre, influencer_active_start, \
        follower_id, follower_name, follower_main_genre, follower_active_start = data

        music_genre.add(influencer_main_genre)
        music_genre.add(follower_main_genre)

        if influencer_main_genre in target_genre and follower_main_genre in target_genre:
            G.add_node(influencer_name, id=influencer_id, genre=influencer_main_genre)
            G.add_node(follower_name, id=follower_id, genre=follower_main_genre)
            G.add_edge(influencer_name, follower_name)

            if influencer_name not in artist2id:
                artist2id[influencer_name] = len(id2artist)
                id2artist.append(influencer_name)

            if follower_name not in artist2id:
                artist2id[follower_name] = len(id2artist)
                id2artist.append(follower_name)

            adj_matrix[artist2id[influencer_name], artist2id[follower_name]] = 1.0

            if adj_matrix[artist2id[follower_name], artist2id[influencer_name]]:
                print(influencer_name, follower_name)
                adj_matrix[artist2id[influencer_name], artist2id[follower_name]] = 0


    # print(G.number_of_nodes())
    # artist_num = len(id2artist)
    # print(adj_matrix)

    identity_matrix = np.identity(artist_num)
    # print(influence_matrix.shape)
    influence_matrix = np.linalg.inv(identity_matrix - alpha * adj_matrix) - identity_matrix
    print(influence_matrix)

    people_influence = np.sum(influence_matrix, 1)
    people_influence_rank = np.argsort(people_influence)


