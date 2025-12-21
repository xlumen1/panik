# PANIK Repository Schema
---
Repositories are stored as `index.db` in a repo's root and are in *SQLITE3* format.
The tables are as follows:

meta: Repository metadata
|key TEXT PRIMARY KEY|value TEXT|
|--------------------|----------|
|schema              |1         |

packages: Package data
|name TEXT NOT NULL|version TEXT NOT NULL|arch TEXT NOT NULL|file TEXT NOT NULL|
|------------------|---------------------|------------------|------------------|

PRIMARY KEY (name, version, arch)

dependenices: Package dependenices
|pkg\_name TEXT NOT NULL|pkg\_version TEXT NOT NULL|dep\_name TEXT NOT NULL|dep\_constraint TEXT|
|-----------------------|--------------------------|-----------------------|--------------------|

tags
|name TEXT PRIMARY KEY|pkg\_name TEXT NOT NULL|pkg\_version TEXT NOT NULL|arch TEXT NOT NULL|
|---------------------|-----------------------|--------------------------|------------------|

FOREIGN KEY (pkg\_name, pkg\_version, arch)
REFERENCES packages(name, version, arch)

Additionallly, these indexes must be maintained:
idx\_pkg\_name on packages:
|name|
|----|

idx\_pkg\_name\_version on packages:
|name|version|
|----|-------|

idx\_pkg\_tags on tags:
|pkg\_name|pkg\_version|arch|
|---------|------------|----|

