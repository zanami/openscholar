# OpenScholar: Stats

Provides a configurable statistics block and a JSON api for sitewide statistics.

## Stats JSON v1

### /stats

#### Parameters

##### `version`

Default value: `1`<br/>
*No other accepted values*

##### `type`

Default value: `"websites"`<br/>
*No other accepted values*

##### `format`

Defaults value: `"default"`<br/>
Accepted values: `"default"` or `"geckoboard"`

`"geckoboard"` returns a properly formatted Type 1 (Number) geckoboard widget.

#### Notes

Stats JSON v1 is the first version of the os_stats API.