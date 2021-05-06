/*****************************************************************************
* Multiscale Universal Interface Code Coupling Library Demo 6                *
*                                                                            *
* Copyright (C) 2017 Y. H. Tang, S. Kudo, X. Bian, Z. Li, G. E. Karniadakis  *
* Copyright (C) 2019 S. Rolfo (*STFC Daresbury laboratory)                   *
*                                                                            *
* This software is jointly licensed under the Apache License, Version 2.0    *
* and the GNU General Public License version 3, you may use it according     *
* to either.                                                                 *
*                                                                            *
* ** Apache License, version 2.0 **                                          *
*                                                                            *
* Licensed under the Apache License, Version 2.0 (the "License");            *
* you may not use this file except in compliance with the License.           *
* You may obtain a copy of the License at                                    *
*                                                                            *
* http://www.apache.org/licenses/LICENSE-2.0                                 *
*                                                                            *
* Unless required by applicable law or agreed to in writing, software        *
* distributed under the License is distributed on an "AS IS" BASIS,          *
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
* See the License for the specific language governing permissions and        *
* limitations under the License.                                             *
*                                                                            *
* ** GNU General Public License, version 3 **                                *
*                                                                            *
* This program is free software: you can redistribute it and/or modify       *
* it under the terms of the GNU General Public License as published by       *
* the Free Software Foundation, either version 3 of the License, or          *
* (at your option) any later version.                                        *
*                                                                            *
* This program is distributed in the hope that it will be useful,            *
* but WITHOUT ANY WARRANTY; without even the implied warranty of             *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
* GNU General Public License for more details.                               *
*                                                                            *
* You should have received a copy of the GNU General Public License          *
* along with this program.  If not, see <http://www.gnu.org/licenses/>.      *
******************************************************************************/

/**
 * @file mui_3d.cpp
 * @author Y. H. Tang
 * @date Jan 20, 2015
 * @brief C++ C wrapper to create 3D MUI uniface.
 *
 */

#include "../../mui.h"

extern "C" {

using namespace mui;

typedef uniface3d                        mui_uniface3d;
typedef sampler_gauss3d<double>          mui_sampler_gauss3d;
typedef sampler_moving_average3d<double> mui_sampler_moving_average3d;
typedef chrono_sampler_exact3d           mui_chrono_sampler_exact3d;
typedef chrono_sampler_mean3d            mui_chrono_sampler_mean3d;
typedef sampler_exact3d<double>            mui_sampler_exact3d;
typedef sampler_nearest_neighbor3d<double> mui_sampler_nearest3d;
typedef sampler_pseudo_nearest_neighbor3d<double> mui_sampler_pseudo_nearest_neighbor3d;
typedef sampler_pseudo_nearest2_linear3d<double> mui_sampler_pseudo_nearest2_linear3d;
typedef geometry::box3d mui_geometry_box3d;
typedef geometry::sphere3d mui_geometry_sphere3d;

// allocator
mui_uniface3d* mui_create_uniface3d( const char *URI ) {
	return new mui_uniface3d( URI );
}

mui_sampler_gauss3d* mui_create_sampler_3d( double r, double h ) {
	return new mui_sampler_gauss3d( r, h );
}

mui_sampler_moving_average3d* mui_create_sampler_moving_average3d( double dx, double dy, double dz ) {
	return new mui_sampler_moving_average3d( point3d(dx,dy,dz) );
}

mui_sampler_exact3d* mui_create_sampler_exact3d(void) {
    return new mui_sampler_exact3d();
}

mui_sampler_nearest3d* mui_create_sampler_nearest3d(void) {
    return new mui_sampler_nearest3d();
}

mui_sampler_pseudo_nearest_neighbor3d* mui_create_sampler_pseudo_nearest_neighbor3d( double h ) {
    return new mui_sampler_pseudo_nearest_neighbor3d( h );
}

mui_sampler_pseudo_nearest2_linear3d* mui_create_sampler_pseudo_nearest2_linear3d( double h ) {
    return new mui_sampler_pseudo_nearest2_linear3d( h );
}

mui_chrono_sampler_exact3d* mui_create_chrono_sampler_exact3d(void) {
	return new mui_chrono_sampler_exact3d;
}

mui_chrono_sampler_mean3d* mui_create_chrono_sampler_mean3d( double past, double future ) {
	return new mui_chrono_sampler_mean3d( past, future );
}

mui_geometry_box3d* mui_create_geometry_box3d(double l1_x, double l1_y, double l1_z, double l2_x, double l2_y, double l2_z){
	return new mui_geometry_box3d(point3d(l1_x,l1_y,l1_z),point3d(l2_x,l2_y,l2_z));
}

mui_geometry_sphere3d* mui_create_geometry_sphere3d(double l1_x, double l1_y, double l1_z, double rr){
	return new mui_geometry_sphere3d(point3d(l1_x,l1_y,l1_z),rr);
}

// deallocator
void mui_destroy_uniface3d( mui_uniface3d *uniface ) {
	delete uniface;
}

void mui_destroy_sampler_3d( mui_sampler_gauss3d* sampler ) {
	delete sampler;
}

void mui_destroy_sampler_moving_average3d( mui_sampler_moving_average3d* sampler ) {
	delete sampler;
}
void mui_destroy_sampler_nearest3d( mui_sampler_nearest3d* sampler ) {
	delete sampler;
}

void mui_destroy_sampler_pseudo_nearest_neighbor3d( mui_sampler_pseudo_nearest_neighbor3d* sampler ) {
	delete sampler;
}

void mui_destroy_sampler_pseudo_nearest2_linear3d( mui_sampler_pseudo_nearest2_linear3d* sampler ) {
	delete sampler;
}

void mui_destroy_chrono_sampler_exact3d( mui_chrono_sampler_exact3d* sampler ) {
	delete sampler;
}

void mui_destroy_chrono_sampler_mean3d( mui_chrono_sampler_mean3d* sampler ) {
	delete sampler;
}

void mui_destroy_geometry_box3d( mui_geometry_box3d* box3d){
	delete box3d;
}

void mui_destroy_geometry_sphere3d( mui_geometry_sphere3d* sphere3d){
	delete sphere3d;
}

// push
void mui_push( mui_uniface3d* uniface, const char *attr, double x, double y, double z, double value ) {
	uniface->push( std::string(attr), point3d(x,y,z), value );
}

// spatial sampler: gaussian
// temporal sampler: exact point
double mui_fetch_gaussian_exact( mui_uniface3d* uniface, const char *attr, double x, double y, double z, double t, mui_sampler_gauss3d *spatial, mui_chrono_sampler_exact3d *temporal ) {
	return uniface->fetch( std::string(attr), point3d(x,y,z), t, *spatial, *temporal );
}
// spatial sampler: exact
// temporal sampler: exact point
double mui_fetch_exact_exact( mui_uniface3d* uniface, const char *attr, double x, double y, double z, double t, mui_sampler_exact3d *spatial, mui_chrono_sampler_exact3d *temporal ) {
	return uniface->fetch( std::string(attr), point3d(x,y,z), t, *spatial, *temporal );
}

// spatial sampler: nearest
// temporal sampler: exact point
double mui_fetch_nearest_exact( mui_uniface3d* uniface, const char *attr, double x, double y, double z, double t, mui_sampler_nearest3d *spatial, mui_chrono_sampler_exact3d *temporal ) {
	return uniface->fetch( std::string(attr), point3d(x,y,z), t, *spatial, *temporal );
}

// spatial sampler: pseudo nearest
// temporal sampler: exact point
double mui_fetch_pseudo_nearest_exact( mui_uniface3d* uniface, const char *attr, double x, double y, double z, double t, mui_sampler_pseudo_nearest_neighbor3d *spatial, mui_chrono_sampler_exact3d *temporal ) {
	return uniface->fetch( std::string(attr), point3d(x,y,z), t, *spatial, *temporal );
}

// spatial sampler: pseudo nearest2 linear
// temporal sampler: exact point
double mui_fetch_pseudo_nearest2_linear_exact( mui_uniface3d* uniface, const char *attr, double x, double y, double z, double t, mui_sampler_pseudo_nearest2_linear3d *spatial, mui_chrono_sampler_exact3d *temporal ) {
	return uniface->fetch( std::string(attr), point3d(x,y,z), t, *spatial, *temporal );
}

// spatial sampler: gaussian
// temporal sampler: mean
double mui_fetch_gaussian_mean( mui_uniface3d* uniface, const char *attr, double x, double y, double z, double t, mui_sampler_gauss3d *spatial, mui_chrono_sampler_mean3d *temporal ) {
	return uniface->fetch( std::string(attr), point3d(x,y,z), t, *spatial, *temporal );
}

// spatial sampler: moving average
// temporal sampler: exact point
double mui_fetch_moving_average_exact( mui_uniface3d* uniface, const char *attr, double x, double y, double z, double t, mui_sampler_moving_average3d *spatial, mui_chrono_sampler_exact3d *temporal ) {
	return uniface->fetch( std::string(attr), point3d(x,y,z), t, *spatial, *temporal );
}

// spatial sampler: moving_average
// temporal sampler: mean
double mui_fetch_moving_average_mean( mui_uniface3d* uniface, const char *attr, double x, double y, double z, double t, mui_sampler_moving_average3d *spatial, mui_chrono_sampler_mean3d *temporal ) {
	return uniface->fetch( std::string(attr), point3d(x,y,z), t, *spatial, *temporal );
}

// announce send/receive using different geometries
void mui_announce_send_span_box(mui_uniface3d* uniface, double t0, double tfin, mui_geometry_box3d *box3d){
	uniface->announce_send_span( t0, tfin , *box3d );
}

void mui_announce_recv_span_box(mui_uniface3d* uniface, double t0, double tfin, mui_geometry_box3d *box3d){
	uniface->announce_recv_span( t0, tfin , *box3d );
}

void mui_announce_send_span_sphere(mui_uniface3d* uniface, double t0, double tfin, mui_geometry_sphere3d *sphere3d){
	uniface->announce_send_span( t0, tfin , *sphere3d );
}

void mui_announce_recv_span_sphere(mui_uniface3d* uniface, double t0, double tfin, mui_geometry_sphere3d *sphere3d){
	uniface->announce_recv_span( t0, tfin , *sphere3d );
}

// commit all data in buffer
void mui_commit( mui_uniface3d* uniface, double t ) {
	uniface->commit( t );
}

// commit all data in buffer and return number of communication
int mui_commit_ranks( mui_uniface3d* uniface, double t ) {
	return uniface->commit( t );
}

// wait for peers
void mui_barrier( mui_uniface3d* uniface, double t ) {
	uniface->barrier( t );
}

// remove obsolete data
void mui_forget( mui_uniface3d* uniface, double first, double last ) {
	uniface->forget( first, last );
}

// set automatic deletion
void mui_set_memory( mui_uniface3d* uniface, double length ) {
	return uniface->set_memory( length );
}

// send a double
void mui_send_double( mui_uniface3d* uniface, const char *attr, double value ) {
        uniface->push( std::string(attr), value );
}

// fetch a double
double mui_fetch_double( mui_uniface3d* uniface, const char *attr ) {
        return uniface->fetch<double>( std::string(attr) );
}

// assign int
void mui_send_int( mui_uniface3d* uniface, const char *attr, int value ) {
        uniface->push( std::string(attr), value );
}

// fetch assigned
int mui_fetch_int( mui_uniface3d* uniface, const char *attr ) {
        return uniface->fetch<int>( std::string(attr) );
}

// split comm
MPI_Comm mui_mpi_split_by_app(void) {
    return mpi_split_by_app();
}

}
