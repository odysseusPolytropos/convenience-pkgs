/**
    Copyright (C) 2016 Jennifer Buehler

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software Foundation,
    Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
**/

#include <urdf2inventor/Helpers.h>

#include <ros/ros.h>

#include <map>
#include <string>
#include <vector>

template<typename MeshFormat>
bool urdf2inventor::FileIO<MeshFormat>::initOutputDir() const
{
    return urdf2inventor::helpers::makeDirectoryIfNeeded(outputDir.c_str())
           && initOutputDirImpl();
}

template<typename MeshFormat>
bool urdf2inventor::FileIO<MeshFormat>::writeMeshFiles(const std::map<std::string, MeshFormat>& meshes,
        const std::string& meshOutputExtension,
        const std::string& meshOutputDirectoryName) const
{
    std::string outputMeshDir =  outputDir + "/" + meshOutputDirectoryName;

    if (!urdf2inventor::helpers::makeDirectoryIfNeeded(outputMeshDir.c_str()))
    {
        ROS_ERROR("Could not create directory %s", outputMeshDir.c_str());
        return false;
    }

    // write the mesh files
    typename std::map<std::string, MeshFormat>::const_iterator mit;
    for (mit = meshes.begin(); mit != meshes.end(); ++mit)
    {
        std::stringstream outFilename;
        outFilename << outputMeshDir << "/" << mit->first << meshOutputExtension;
        if (!urdf2inventor::helpers::writeToFile(mit->second, outFilename.str()))
        {
            ROS_ERROR("Could not write file %s", outFilename.str().c_str());
            return false;
        }
    }

    return true;
}


template<typename MeshFormat>
bool urdf2inventor::FileIO<MeshFormat>::write(const ConversionResultPtr& data) const
{
    // First of all, see if we can create output directory
    if (!initOutputDir())
    {
        ROS_ERROR("Can't make directory %s", outputDir.c_str());
        return false;
    }

    if (!writeMeshFiles(data->meshes, data->meshOutputExtension, data->meshOutputDirectoryName))
    {
        ROS_ERROR("Could not write mesh files");
        return false;
    }

    return writeImpl(data);
}
