# HTTP APIs

Timelord provides APIs let web interface retrieve necessary data.

## Summary

**Path** `/api/summary`

### Request

|Name|Type|Note|
|-|-|-|
|hours|int|Get summary data for the past hours|

### Response

Return a json object with the following fields.

|Name|Type|Note|
|-|-|-|
|num_blocks|int|How many blocks invoved|
|hours|int|The summary data for the past hours|
|high_height|int|The height range with the highest number of height|
|low_height|int|The height range with the lowest number of height|
|avg_duration|int|The number of seconds for producing every block|
|summary|array|Block counts for different time periods|

The json object from `summary` is declared here:

|Name|Type|Note|
|-|-|-|
|min|int|This period with the count number of blocks with time produced within this minutes|
|count|int|The number of blocks|

## Status

Return a json object contains the timelord current status.

### Request

### Response

|Name|Type|Note|
|-|-|-|
|challenge|string|Current challenge from the chain|
|difficulty|int64|Current difficulty from the chain|
|estimated_netspace|int|The number of network space estimated (in TB)|
|fork_height|int|The height of hard-fork of BHDIP009|
|height|int|Current chain height|
|iters_per_sec|int|The vdf speed|
|last_block_info|object|The last block information|
|max_size|int64|The largest net space connected to this timelord|
|num_connections|int|The number of connections currently are connected to this timelord|
|pledge_info|int|The objects about pledge periods|
|server_ip|string|The ip address of the timelord|
|status_string|string|The string of "good" represents the everything of the timelord is ok|
|supply|object|The supply info. is queried from the server|
|total_size|int64|The summary number of the size reported from connected clients|
|vdf_pack|object|The vdf details|

`last_block_info`

|Name|Type|Note|
|-|-|-|
|accumulate|int|The accumulated amount from the chain (in DePC)|
|address|string|The address of the miner who posted the block|
|block_difficulty|int64|The difficulty value of the block|
|challenge|string|The challenge for the block|
|challenge_difficulty|string|The difficulty of the challenge|
|farmer_pk|string|The public-key of the farmer|
|filter_bits|int|The filter bits is a trottle valve to control the IO read frequence|
|hash|string|The hash value of the block|
|height|int|The height of the block|
|reward|float|The reward of the block|
|vdf_iters|int|The number of iters consumed for the block|
|vdf_iters_req|int|The number of iters required for the block|
|vdf_speed|int|Iters per second can be calculated from this timelord|
|vdf_time|int|How long is the time for calculating the iters|

`pledge_info`

|Name|Type|Note|
|-|-|-|
|capacity_eval_window|int|The number of blocks to eval the capacity|
|retarget_min_heights|int|The number of heights respect by next retargeting pledge|
|pledges|array|The terms of the pledges|

`term`

|Name|Type|Note|
|-|-|-|
|actual_percent|int|The value times with pledge amount and divide by 100 will get the result of actual amount|
|lock_height|int|The locked height|

`supply`

|Name|Type|Note|
|-|-|-|
|calc|object|The supply details should be used to calculate rewards and etc|
|last|object|The supply details from the last block|

`supply detail`

|Name|Type|Note|
|-|-|-|
|actual|int|Actual pledge amount|
|burned|int|The amount burned from the chain|
|height|int|The details are calculated from height|
|total|int|Total amount are pledged to the chain|

`vdf_pack`

|Name|Type|Note|
|-|-|-|
|challenge|string|The challenge for the vdf|
|height|int|The height to calculate|
|requests|array|The vdf requested from clients|
|timestamp|int|Unix timestamp when the vdf begins|

## Netspace

The data to draw the graph for the difficulty status of the chain.

### Request

|Name|Type|Note|
|-|-|-|
|hours|int|The data range from the number of past hours to now|

### Response

The response is an array contains the objects for the difficulty and heights.

|Name|Type|Note|
|-|-|-|
|block_difficulty|int|The difficulty value from the height|
|challenge_difficulty|int|The difficulty value of the challenge|
|height|int|The height|
|netspace|int|The netspace values are collected by the timelord|

## Rank

Query the miners from a period.

### Request

|Name|Type|Note|
|-|-|-|
|hours|int|The hours represents the period of the mining miners|

## Response

|Name|Type|Note|
|-|-|-|
|begin_height|int|The miners since the height|
|entries|array|The details of the miners|

`entry`

|Name|Type|Note|
|-|-|-|
|average_difficulty|int|The value of average difficulty of the miner|
|count|int|The number of blocks are mined by the miner|
|farmer_pk|string|The public-key of the miner|
|total_reward|int64|The amount received by the miner in the period|
