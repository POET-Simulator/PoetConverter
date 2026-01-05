using EzXML
using HDF5

# --- Configuration ---
h5_filename = "dolo_fgcs_3_skip.h5"
xdmf_filename = "dolo_fgcs_3_skip.xmf"

# Grid settings (Adjust these if your simulation has specific physical dimensions)
# Default values for a 2D grid with unit spacing
origin = [0.0, 0.0]   # X, Y origin
spacing = [1.0, 1.0]  # dx, dy

# --- Helper Function: Sort Iterations ---
# Extracts the number from "iteration_100" to sort numerically (0, 1, 20, 100...)
function get_iteration_number(name)
    m = match(r"iteration_(\d+)", name)
    return m === nothing ? nothing : parse(Int, m.captures[1])
end

# --- Main Processing ---
function create_xdmf()
    # 1. Open HDF5 to inspect structure
    if !isfile(h5_filename)
        println("Error: File $h5_filename not found.")
        return
    end

    h5f = h5open(h5_filename, "r")

    # Get all keys and filter for "iteration_" groups
    all_keys = keys(h5f)
    iter_groups = filter(k -> startswith(k, "iteration_"), all_keys)

    # Sort groups by the integer value of the iteration
    sort!(iter_groups, by=get_iteration_number)

    if isempty(iter_groups)
        println("No iteration groups found.")
        close(h5f)
        return
    end

    # Get dimensions from the first dataset of the first iteration to setup Topology
    # We look for a known species like "C" or just the first valid dataset
    first_group = h5f[iter_groups[1]]
    # Pick the first dataset key to check dims
    sample_ds_name = keys(first_group)[1]
    sample_ds = first_group[sample_ds_name]
    # HDF5.jl reads dims as (X, Y) usually, but XDMF writes "Y X" (row-major)
    # Extract dimensions from the dataset
    dims = size(sample_ds)
    # Format dimensions string for XDMF (reverse for standard C-order visualization if needed, 
    # but for 400x400 it looks the same. XDMF usually wants "NY NX")
    dim_str = join(reverse(dims), " ")

    # 2. Start constructing XML
    doc = XMLDocument()
    root = ElementNode("Xdmf")
    root["Version"] = "3.0"
    setroot!(doc, root)

    domain = addelement!(root, "Domain")

    # Time Collection Grid (The wrapper for the time series)
    grid_collection = addelement!(domain, "Grid")
    grid_collection["Name"] = "Chemical_Evolution"
    grid_collection["GridType"] = "Collection"
    grid_collection["CollectionType"] = "Temporal"

    println("Generating XDMF for $(length(iter_groups)) time steps...")

    for group_name in iter_groups
        iter_num = get_iteration_number(group_name)

        # --- Create Grid for this Timestep ---
        grid = addelement!(grid_collection, "Grid")
        grid["Name"] = group_name
        grid["GridType"] = "Uniform"

        # --- Time ---
        time = addelement!(grid, "Time")
        time["Value"] = string(iter_num)

        # --- Topology (The Mesh) ---
        # 2DCoRectMesh implies a structured grid defined by Origin + Spacing
        topo = addelement!(grid, "Topology")
        topo["TopologyType"] = "2DCoRectMesh"
        topo["Dimensions"] = dim_str

        # --- Geometry (Physical Coordinates) ---
        geo = addelement!(grid, "Geometry")
        geo["GeometryType"] = "ORIGIN_DXDY"

        # Origin
        data_origin = addelement!(geo, "DataItem")
        data_origin["Name"] = "Origin"
        data_origin["Dimensions"] = "2"
        data_origin["NumberType"] = "Float"
        data_origin["Format"] = "XML"
        setnodecontent!(data_origin, join(origin, " "))

        # Spacing
        data_spacing = addelement!(geo, "DataItem")
        data_spacing["Name"] = "Spacing"
        data_spacing["Dimensions"] = "2"
        data_spacing["NumberType"] = "Float"
        data_spacing["Format"] = "XML"
        setnodecontent!(data_spacing, join(spacing, " "))

        # --- Attributes (Chemical Species) ---
        # Iterate over all datasets inside the current iteration group
        current_group = h5f[group_name]
        for ds_name in keys(current_group)
            # We assume all items in the group are the 2D arrays (datasets)
            # Construct the path: /iteration_0/C
            h5_path = "/" * group_name * "/" * ds_name

            attr = addelement!(grid, "Attribute")
            attr["Name"] = ds_name
            attr["AttributeType"] = "Scalar"
            attr["Center"] = "Node" # Assuming 400x400 data points match 400x400 mesh nodes

            data_item = addelement!(attr, "DataItem")
            data_item["Format"] = "HDF"
            data_item["Dimensions"] = dim_str
            # Precision can be detected, but 8 (double) or 4 (float) is standard. 
            # Using 8 for 64-bit floating-point (double precision)
            data_item["NumberType"] = "Float"
            data_item["Precision"] = "8"

            # The text content is the file path : internal path
            setnodecontent!(data_item, h5_filename * ":" * h5_path)
        end
    end

    # 3. Write to file
    write(xdmf_filename, doc)
    println("Successfully created $xdmf_filename")

    close(h5f)
end

create_xdmf()