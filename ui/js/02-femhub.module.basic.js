
Ext.ns("FEMhub.ModuleBasic");

FEMhub.ModuleBasic.BasicWindow = Ext.extend(FEMhub.ModuleBasicWindowUi, {
    initComponent: function() {
        FEMhub.ModuleBasic.BasicWindow.superclass.initComponent.call(this);
        this.my_init();
        this.mesh_editor = new FEMhub.MeshEditor();
        this.geometry_tab.add(this.mesh_editor);
        this.help_tab.add(this.help_panel);
        this.model_parameters.add([this.boundary_conditions_window, this.materials_window]);
        this.arrayBoundaryConditions = new Array();

        this.panel_change_bottombar_to_statusbar(this.computation_tab);
        this.statusbar = this.computation_tab.getBottomToolbar();
        this.engine = new FEMhub.ModuleBasic.Engine({
            statusbar: this.statusbar
        });
        this.calculate_button.on("click", this.calculate, this);
        //this.dirichlet_save_button.on("click", this.save_bc_dirichlet, this);
        //this.neumann_save_button.on("click", this.save_bc_neumann, this);
        //this.newton_save_button.on("click", this.save_bc_newton, this);

        this.mesh_editor.on("mesh_submitted", function() {
            FEMhub.log("Mesh saved. Turning into the Model tab");
            this.main_tabs.setActiveTab(1);
        }, this);
        this.main_tabs.on("tabchange", function () {
            FEMhub.log("Tab changed, refreshing the flex mesh editor.");
            this.mesh_editor.reload_flash();
        }, this);

        this.view = new FEMhub.views.WebGLViewComponent();
        this.computation_result.add(this.view);
    },

    save_bc_dirichlet: function ()
    {
        // load parameters here
        var value = 0; //var value = this.dirichlet_bc_form.getValue(0);
        var marker = 34; //var marker = this.dirichlet_bc_marker.getValue(1);
        FEMhub.log("Added new Dirichlet BC, value = " + value + ", marker = " + marker);

        // Create new boundary condition.
        //var newBC = new boundaryCondition(marker, type, value, -9999);
        //this.arrayBoundaryConditions.push(newBC);
    },

    save_bc_neumann: function ()
    {
    },

    save_bc_newton: function ()
    {
    },

    panel_change_bottombar_to_statusbar: function(panel) {
        // This changes the bottom bar of 'panel' into FEMhub Statusbar
        // This uses a little hack, depending on the exact internals of ExtJS
        // Panel class.
        var idx = panel.toolbars.lastIndexOf(panel.bottomToolbar);
        panel.bottomToolbar = new FEMhub.Statusbar({
                busyText: '',
                defaultText: ''
            });
        panel.toolbars[idx] = panel.bottomToolbar;
    },

    my_init: function () {
        // window to display boundary conditions
        this.boundary_conditions_window = new Ext.Panel({
            title: 'Boundary conditions',
            margins:'3 10 3 3',
            cmargins:'3 3 3 3',
            items: [{
        xtype: 'box',
                height: '10',
                html: ""
        }, {
        xtype: 'box',
                html: "Dirichlet markers:"
        }, {
                xtype: 'textfield',
                id: "BC_dir_marker",
                value: '4'
        }, {
        xtype: 'box',
                html: "Dirichlet values:"
        }, {
                xtype: 'textfield',
                id: "BC_dir_value",
                value: '0'
                }, {
        xtype: 'box',
                html: "Neumann markers:"
        }, {
                xtype: 'textfield',
                id: "BC_neumann_marker",
                value: '1, 3'
        }, {
        xtype: 'box',
                html: "Neumann values:"
        }, {
                xtype: 'textfield',
                id: "BC_neumann_value",
                value: '0, 0'
        }, {
        xtype: 'box',
                html: "Newton markers:"
        }, {
                xtype: 'textfield',
                id: "BC_newton_marker",
                value: '2'
        }, {
        xtype: 'box',
                html: "Newton values:"
        }, {
                xtype: 'textfield',
                id: "BC_newton_value",
                value: '(1, 1)'
        }, {
        xtype: 'box',
                height: '10',
                html: ""
        }]
        });

        // window to display equation parameters
        this.materials_window = new Ext.Panel({
            title: 'Equation data',
            margins:'3 10 3 3',
            cmargins:'3 3 3 3',

            items: [{
        xtype: 'box',
                height: '10',
                html: ""
        }, {
        xtype: 'box',
                html: "Material markers:"
        }, {
                xtype: 'textfield',
                id: "Mat_marker",
                value: '0'
        }, {
        xtype: 'box',
                html: "Const c1:"
        }, {
                xtype: 'textfield',
                id: "Mat_c1",
                value: '1'
        }, {
        xtype: 'box',
                html: "Const c2:"
        }, {
                xtype: 'textfield',
                id: "Mat_c2",
                value: '0'
        }, {
        xtype: 'box',
                html: "Const c3:"
        }, {
                xtype: 'textfield',
                id: "Mat_c3",
                value: '0'
        }, {
        xtype: 'box',
                html: "Const c4:"
        }, {
                xtype: 'textfield',
                id: "Mat_c4",
                value: '0'
        }, {
        xtype: 'box',
                html: "Const c5:"
        }, {
                xtype: 'textfield',
                id: "Mat_c5",
                value: '1'
        }, {
        xtype: 'box',
                height: '10',
                html: ""
        }]
        });

        this.help_panel = new Ext.Panel({
            listeners: {
            afterrender: function(panel) {
            FEMhub.RPC.Docutils.render({rst:
"Module Basic Adapt\n" +
"==================\n",
                            /*
This is a simple generic module for a linear second-order PDE based on the Hermes library.
The purpose of modules like this is to provide a higher-level API where the user is not
exposed to wesk forms, spaces, solver initialization, and other technical details.

PDE
===

.. math::

    -div(c_1 \nabla u) + (c_2, c_3) \cdot \nabla u + c_4 u = c_5

Here $c_1$, $c_2$, ..., $c_5$ are element-wise constant equation parameters.

Boundary conditions
===================

  * Dirichlet with piecewise-constant values ($u$ = const),
  * Neumann with piecewise-constant normal derivatives ($\partial u/ \partial n$ = const),
  * Newton with piecewise-constant parameters (const_1 $u$ + $\partial u/ \partial n$ = const_2)'
                                                     */
                }, {
                    okay: function(result) {
                        FEMhub.log(result.html);
                        panel.update(result.html);
                    }
                }, {
                    fail: function(reason, result) {
                          FEMhub.log(reason);
                    }
                });
            }
            }
        });
    },

    calculate: function() {
        // Input box for initial poly degree.
        Init_p_val = this.computation_setup.get("Init_p").getValue();
        FEMhub.log("Init_p_val: " + Init_p_val);

        // Input box for initial refinements.
        Init_ref_num_val = this.computation_setup.get("Init_ref_num").getValue();
        FEMhub.log("Init_ref_num_val: " + Init_ref_num_val);

        // Input box for matrix solver.
        //Matrix_solver_val = this.computation_setup.get("Matrix_solver").getValue();
        //FEMhub.log("Matrix solver: " + Matrix_solver_val);

        // Get matrix solver from the group of radio buttons.
        Matrix_solver_val = FEMhub.ModuleBasic.get_radio_group_value(this.computation_setup.getComponent('ms_field_set'));
        FEMhub.log("Matrix_solver_val: " + Matrix_solver_val);

        // Input box for Dirichlet markers.
        BC_dir_marker_val = this.boundary_conditions_window.get("BC_dir_marker").getValue();
        FEMhub.log("BC_dir_marker_val: " + BC_dir_marker_val);

        // Input box for Dirichlet values.
        BC_dir_value_val = this.boundary_conditions_window.get("BC_dir_value").getValue();
        FEMhub.log("BC_dir_value_val: " + BC_dir_value_val);

        // Input box for Neumann markers.
        BC_neumann_marker_val = this.boundary_conditions_window.get("BC_neumann_marker").getValue();
        FEMhub.log("BC_neumann_marker_val: " + BC_neumann_marker_val);

        // Input box for Neumann values.
        BC_neumann_value_val = this.boundary_conditions_window.get("BC_neumann_value").getValue();
        FEMhub.log("BC_neumann_value_val: " + BC_neumann_value_val);

        // Input box for Newton markers.
        BC_newton_marker_val = this.boundary_conditions_window.get("BC_newton_marker").getValue();
        FEMhub.log("BC_newton_marker_val: " + BC_newton_marker_val);

        // Input box for Newton values.
        BC_newton_value_val = this.boundary_conditions_window.get("BC_newton_value").getValue();
        FEMhub.log("BC_newton_value_val: " + BC_newton_value_val);

        // Input box for material markers.
        Mat_marker_val = this.materials_window.get("Mat_marker").getValue();
        FEMhub.log("Mat_marker_val: " + Mat_marker_val);

        // Input box for equation constants c1.
        Mat_c1_val =  this.materials_window.get("Mat_c1").getValue();
        FEMhub.log("Mat_c1_val: " + Mat_c1_val);

        // Input box for equation constants c2.
        Mat_c2_val =  this.materials_window.get("Mat_c2").getValue();
        FEMhub.log("Mat_c2_val: " + Mat_c2_val);

        // Input box for equation constants c3.
        Mat_c3_val =  this.materials_window.get("Mat_c3").getValue();
        FEMhub.log("Mat_c3_val: " + Mat_c3_val);

        // Input box for equation constants c4.
        Mat_c4_val =  this.materials_window.get("Mat_c4").getValue();
        FEMhub.log("Mat_c4_val: " + Mat_c4_val);

        // Input box for material constants c5.
        Mat_c5_val =  this.materials_window.get("Mat_c5").getValue();
        FEMhub.log("Mat_c5_val: " + Mat_c5_val);

        // Source code (first and second part --- the mesh is inserted in
        // between using this.mesh_editor.get_mesh(), see below)
        this.sourcecode = FEMhub.join(
"from hermes2d.hermes2d import Linearizer",
"#from hermes2d.plot import sln2png, plot_sln_mayavi",
"#from femhub.plot import return_mayavi_figure",
"from basic import ModuleBasic",
"",
"mesh = '''",
"{0}",
"'''",
"",
"def get_sln():",
"    e = ModuleBasic()",
"    mesh_ok = e.set_mesh_str(mesh)",
"    # assert mesh_ok is True",                                // XXX: why this doesn't work?
"    e.set_initial_mesh_refinement(" + Init_ref_num_val + ")",
"    e.set_initial_poly_degree(" + Init_p_val + ")",
"    e.set_matrix_solver('" + Matrix_solver_val + "')",
"    e.set_material_markers([" + Mat_marker_val + "])",
"    e.set_c1_array([" + Mat_c1_val + "])",
"    e.set_c2_array([" + Mat_c2_val + "])",
"    e.set_c3_array([" + Mat_c3_val + "])",
"    e.set_c4_array([" + Mat_c4_val + "])",
"    e.set_c5_array([" + Mat_c5_val + "])",
"    e.set_dirichlet_markers([" + BC_dir_marker_val + "])",
"    e.set_dirichlet_values([" + BC_dir_value_val + "])",
"    e.set_neumann_markers([" + BC_neumann_marker_val + "])",
"    e.set_neumann_values([" + BC_neumann_value_val + "])",
"    e.set_newton_markers([" + BC_newton_marker_val + "])",
"    e.set_newton_values([" + BC_newton_value_val + "])",
"    success = e.calculate()",
"    assert success is True",
"    print 'Computation finished.'",
"    sln = e.get_solution()",
"    f = open('application.log')",
"    print f.read()",
"    return sln",
"",
"def plot_sln(sln):",
"    lin = Linearizer()",
"    lin.process_solution(sln)",
"    lin.save_data_json('dataset.json', True)",
"    f = open('dataset.json')",
"    print f.read()");
//"",
//"    fig = plot_sln_mayavi(sln, offscreen=True)",
//"    return_mayavi_figure(fig)");

        this.sourcecode_generated = String.format(this.sourcecode, this.mesh_editor.get_mesh());

        this.engine.evaluate({
            source: this.sourcecode_generated,
            statusbar_text: "Importing...",
            done: function (result) {
                this.engine.evaluate({
                    source: "sln = get_sln()",
                    statusbar_text: "Calculating...",
                    done: function (result) {
                        var d_log = this.computation_log;
                        var output = result.out;
                        d_log.update('<b>Computation log</b>:<br><br><pre>' + output + '</pre>');
                        this.engine.evaluate({
                            source: "plot_sln(sln)",
                            statusbar_text: "Plotting...",
                            done: this.display_results,
                            scope: this
                        });
                    },
                    scope: this
                });
            },
            scope: this
        });
    },

    display_results: function(result) {
        FEMhub.log("Results received");
        //FEMhub.log(result);
        //this.main_tabs.setActiveTab(3);
        var d_log = this.computation_log;
        if (result.traceback_html) {
            //FEMhub.msg.error("Python traceback", result.traceback_html);
            d_log.update("Python traceback:<br/><pre>" + result.traceback_html + "</pre>");
            FEMhub.log(result.traceback_html);
        } else {
            var dataset = result.out;
            this.view.plot(dataset);
            //var data = result.plots[0].data;
            //d_res = this.computation_result;
            //d_res.update('<table height="100%" width="100%" border="0"><tr><td valign="middle" align="center"><img src="data:image/png;base64,' + data + '"/></td></tr></table>');
        }
    }

});

FEMhub.Modules.ModuleBasic = Ext.extend(FEMhub.Module, {
    launcher: {
        text: 'Basic',
        icon: 'femhub-module-basic-launcher-icon'
    },
    winCls: FEMhub.ModuleBasic.BasicWindow
});

FEMhub.ModuleBasic.get_radio_group_value = function(radiofieldset) {
    FEMhub.log("Reading matrix solver selection.");
    for (i = 0; i < radiofieldset.items.length; i++) {
        var item = radiofieldset.getComponent(i);
        if (item.checked) {
            FEMhub.log(item.boxLabel + " radio is checked.");
            return item.value;
        }
    }
    return false;
}

FEMhub.ModuleBasic.Engine = Ext.extend(Ext.util.Observable, {
    constructor: function(config) {
        this.addEvents("engine_initialized");
        this.addEvents("evaluate_finished");
        FEMhub.ModuleBasic.Engine.superclass.constructor.call(this, config);

        this.uuid = 0;
        this.initialized = false;
        this.statusbar = config.statusbar;
    },

    isInitialized: function () {
        return this.initialized;
    },

    initialize: function(config) {
        if (this.isInitialized())
            return;
        config = config || {};
        this.uuid = FEMhub.util.rfc.UUID();
        FEMhub.log("UUID created" + this.uuid);
        FEMhub.RPC.Engine.init({uuid: this.uuid}, {
            okay: function(result) {
                FEMhub.log("Engine initialized");
                this.initialized = true;
                this.fireEvent("engine_initialized");
                if (Ext.isDefined(config.done)) {
                    config.done.call(config.scope);
                }
            },
            fail: function(reason, result) {
                FEMhub.msg.info("Engine failed to initialize");
                FEMhub.log("Engine failed to initialize");
            },
            scope: this,
            status: {
                start: function() {
                    if (Ext.isDefined(this.statusbar))
                        this._id = this.statusbar.showBusy({text: "Initializing the engine"});
                },
                end: function(ok, ret) {
                    if (Ext.isDefined(this.statusbar))
                        this.statusbar.clearBusy(this._id);
                }
            }
        });
    },

    evaluate: function(config) {
        config = config || {};
        if (! this.isInitialized()) {
            this.initialize({
                done: function () {
                    this.evaluate(config);
                },
                scope: this
            });
            return;
        }
        FEMhub.RPC.Engine.evaluate({
            uuid: this.uuid,
            source: config.source
        }, {
            okay: function(result) {
                //FEMhub.log("CODE FOR ENGINE:");
                //FEMhub.log(config.source);
                FEMhub.log("Evaluate succeeded");
                this.fireEvent("evaluate_finished", result);
                if (Ext.isDefined(config.done)) {
                    config.done.call(config.scope, result);
                }
            },
            fail: function(reason) {
                FEMhub.log("Evaluate failed: " + reason);
            },
            scope: this,
            status: {
                start: function() {
                    if (Ext.isDefined(this.statusbar))
                        this._id = this.statusbar.showBusy({
                            text: config.statusbar_text
                        });
                },
                end: function(ok, ret) {
                    if (Ext.isDefined(this.statusbar))
                        this.statusbar.clearBusy(this._id);
                },
                scope: this
            }
        });
    }
});

