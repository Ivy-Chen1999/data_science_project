import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

def find_safe_car():
    df = pd.read_csv("Safercar_data.csv", header=0)
    df['ROLLOVER_STARS'] = pd.to_numeric(df['ROLLOVER_STARS'], errors='coerce')
    df['OVERALL_STARS'] = pd.to_numeric(df['OVERALL_STARS'], errors='coerce')
    df = df.dropna(subset=['ROLLOVER_STARS', 'OVERALL_STARS']) 
    sorted_df = df[['MAKE', 'MODEL', 'ROLLOVER_STARS', 'OVERALL_STARS']].sort_values(by=['OVERALL_STARS', 'ROLLOVER_STARS'], ascending=False)
    rollover_mean = df['ROLLOVER_STARS'].mean()
    overall_mean = df['OVERALL_STARS'].mean()
    tesla_df = df[df['MAKE'] == 'TESLA'][['MAKE', 'MODEL', 'ROLLOVER_STARS', 'OVERALL_STARS']]
    tesla_ranking = tesla_df.sort_values(by=['OVERALL_STARS', 'ROLLOVER_STARS'], ascending=False)
    tesla_rollover_rank = df['ROLLOVER_STARS'].rank(ascending=False).loc[df['MAKE'] == 'TESLA'].iloc[0]
    tesla_overall_rank = df['OVERALL_STARS'].rank(ascending=False).loc[df['MAKE'] == 'TESLA'].iloc[0]

    brand_avg_scores = df.groupby('MAKE')[['ROLLOVER_STARS', 'OVERALL_STARS']].mean().sort_values(by='OVERALL_STARS', ascending=False)
    
    plt.figure(figsize=(10, 6))
    brand_avg_scores.plot(kind='bar', figsize=(12, 6))
    plt.title('Average Rollover and Overall Scores by Car Brand')
    plt.ylabel('Average Stars')
    plt.xlabel('Car Brand')
    plt.xticks(rotation=45, ha='right')
    plt.tight_layout()
    plt.show()
    return sorted_df, rollover_mean, overall_mean, tesla_ranking, tesla_overall_rank, tesla_rollover_rank

def main():
    sorted_df, rollover_mean, overall_mean, tesla_ranking, tesla_overall_rank, tesla_rollover_rank = find_safe_car()
    print("Sorted Car Data:\n", sorted_df.head().to_markdown(index=False))
    print("\nRollover Stars Mean:", rollover_mean)
    print("Overall Stars Mean:", overall_mean)
    print("\nTesla Ranking:\n", tesla_ranking.to_markdown(index=False))



main()

