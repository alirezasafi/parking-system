import './App.css';
import React, {Component} from "react";
import "bootstrap/dist/css/bootstrap.min.css";
import {Container, Row, Navbar, InputGroup, FormControl, Col, Button} from 'react-bootstrap';
import Pelak from './assets/pelak.png';
import ReserveApi from "./api";
import Swal from "sweetalert2";

class App extends Component {

    state = {
        carID: null,
        firstSection: null,
        secondSection: null,
        thirdSection: null,
        forthSection: null,
    }

    async reserveCar() {
        const id = this.state.firstSection + this.state.secondSection + this.state.thirdSection;
        const response = await ReserveApi.reserveCar(id);
        if(response.ok) {
            Swal.fire({
                position: 'center',
                type: 'success',
                title: 'موفق',
                showConfirmButton: true,
            })

        }else{
            console.log("not fetched!")
        }
    }


    handleChange = (name, e) => {
        this.setState(prevState => {
            const newState = {...prevState};
            newState[name] = e.target.value;
            return newState;
        });
    };


    render() {
        return (
            <div>
                <div className="background"></div>
                <Navbar style={{backgroundColor: "rgba(0,0,179,0.70)"}}>
                    <Navbar.Text className="header-texts">سیستم‌های نهفته و بی‌درنگ</Navbar.Text>
                    <Navbar.Toggle/>
                    <Navbar.Collapse className="justify-content-end">
                        <Navbar.Text className="header-texts">
                            سیستم پارکینگ
                        </Navbar.Text>
                    </Navbar.Collapse>
                </Navbar>
                <Container className="mt-5">
                    <Row className="justify-content-center">
                        <img src={Pelak} alt="pelak"/>
                    </Row>
                    <Row className="justify-content-center align-items-center mt-4">
                        <span className="main-text">لطفا پلاک خود را وارد نمایید.</span>
                    </Row>
                    <Row className="justify-content-center id-input align-items-center mt-5">
                        <Col s={2}>
                            <InputGroup className="mb-3 iransans-font">
                                <FormControl
                                    placeholder="12"
                                    onChange={(value) => this.handleChange('firstSection', value)}
                                />
                            </InputGroup>
                        </Col>
                        <Col s={1}>
                            <InputGroup className="mb-3 iransans-font">
                                <FormControl
                                    placeholder="ب"
                                    onChange={(value) => this.handleChange('secondSection', value)}
                                />
                            </InputGroup>
                        </Col>
                        <Col s={3}>
                            <InputGroup className="mb-3 iransans-font">
                                <FormControl
                                    placeholder="365"
                                    onChange={(value) => this.handleChange('thirdSection', value)}
                                />
                            </InputGroup>
                        </Col>
                        <Col s={1}>
                            <Row className="justify-content-center">
                                <span className="main-text">ایران</span>
                            </Row>
                        </Col>
                        <Col s={2}>
                            <InputGroup className="mb-3 iransans-font">
                                <FormControl
                                    placeholder="11"
                                    onChange={(value) => this.handleChange('forthSection', value)}
                                />
                            </InputGroup>
                        </Col>
                    </Row>
                    <Row className="justify-content-center align-items-center">
                        <Button disabled={
                            !this.state.firstSection
                            || !this.state.secondSection
                            || !this.state.thirdSection
                            || !this.state.forthSection
                        } onClick={() => this.reserveCar()} variant="warning"
                                className="reserve-button">رزرو</Button>
                    </Row>
                </Container>
            </div>
        );
    }
}

export default App;